"""
General python utilities.
"""
import os
import random
import re
import shutil
import string
import subprocess
import numpy as np

from symforce import logger
from symforce import sympy as sm
from symforce import types as T


def remove_if_exists(path):
    # type: (str) -> None
    """
    Delete a file or directory if it exists.
    """
    if not os.path.exists(path):
        logger.debug("Doesn't exist: {}".format(path))
        return
    elif os.path.isdir(path):
        logger.debug("Removing directory: {}".format(path))
        shutil.rmtree(path)
    else:
        logger.debug("Removing file: {}".format(path))
        os.remove(path)


def execute_subprocess(
    cmd,  # type: T.Union[str, T.Sequence[str]]
    stdin_data=None,  # type: T.Optional[str]
    log_stdout=True,  # type: bool
    log_stdout_to_error_on_error=True,  # type: bool
    **kwargs  # type: T.Any
):
    # type: (...) -> unicode
    """
    Execute subprocess and log command as well as stdout/stderr.

    Args:
        stdin_data (bytes): Data to pass to stdin
        log_stdout (bool): Write process stdout to the logger?
        log_stdout_to_error_on_error: Write output to logger.error if the command fails?

    Raises:
        subprocess.CalledProcessError: If the return code is nonzero
    """
    if stdin_data is not None:
        stdin_data_encoded = stdin_data.encode("utf-8")
    else:
        stdin_data_encoded = bytes()

    cmd_str = " ".join(cmd) if isinstance(cmd, (tuple, list)) else cmd
    logger.info("Subprocess: {}".format(cmd_str))

    proc = subprocess.Popen(cmd, stdin=subprocess.PIPE, stdout=subprocess.PIPE, stderr=subprocess.STDOUT, **kwargs)  # type: ignore
    (stdout, _) = proc.communicate(stdin_data_encoded)

    going_to_log_to_err = proc.returncode != 0 and log_stdout_to_error_on_error

    stdout_decoded = stdout.decode("utf-8")
    if log_stdout and not going_to_log_to_err:
        logger.info(stdout_decoded)

    if going_to_log_to_err:
        logger.error(
            "Subprocess {} exited with code: {}.  Output:\n{}".format(
                cmd, proc.returncode, stdout_decoded
            )
        )

    if proc.returncode != 0:
        raise subprocess.CalledProcessError(proc.returncode, cmd, stdout)

    return stdout_decoded


def camelcase_to_snakecase(s):
    # type: (str) -> str
    """
    Convert CamelCase -> snake_case.
    """
    return re.sub(r"(?<!^)(?=[A-Z][a-z])", "_", s).lower()


def files_in_dir(dirname, relative=False):
    # type: (str, bool) -> T.Iterator[str]
    """
    Return a list of files in the given directory.
    """
    for root, _, filenames in os.walk(dirname):
        for filename in filenames:
            abspath = os.path.join(root, filename)
            if relative:
                yield os.path.relpath(abspath, dirname)
            else:
                yield abspath


def id_generator(size=6, chars=string.ascii_uppercase + string.digits):
    # type: (int, str) -> str
    """
    Generate a random string within a character set - for example "6U1S75".
    This is not cryptographically secure.
    """
    return "".join(random.choice(chars) for _ in range(size))


def get_type(a):
    # type: (T.Any) -> T.Type
    """
    Returns the type of the element if its an instance, or a pass through if already a type.
    """
    if isinstance(a, type):
        return a
    else:
        return type(a)


def scalar_like(a):
    # type: (T.Any) -> bool
    """
    Returns whether the element is scalar-like (an int, float, or sympy expression).

    This method does not rely on the value of a, only the type.
    """
    a_type = get_type(a)
    if issubclass(a_type, (int, float, np.float32, np.float64)):
        return True
    is_expr = issubclass(a_type, sm.Expr)
    is_matrix = issubclass(a_type, sm.MatrixBase) or (hasattr(a, "is_Matrix") and a.is_Matrix)
    return is_expr and not is_matrix


def is_valid_variable_name(name):
    # type: (str) -> bool
    """
    Returns true if name does not contain whitespace, has only alphanumeric characters,
    and does not start with a number.
    """
    return re.match(".*\s.*|.*\W.*|^\d", name) is None
