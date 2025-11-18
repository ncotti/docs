"""Script created to test Ruff."""  # noqa: INP001

import logging


def my_fun(a: str) -> None:
    """My fun."""


logging.basicConfig(level=logging.INFO)
logger = logging.getLogger(__name__)

logger.info("Hello world")

a = 5
b = 9

logger.info(a)
