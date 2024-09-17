from setuptools import find_packages
from setuptools import setup

setup(
    name='aprs_vision',
    version='0.0.0',
    packages=find_packages(
        include=('aprs_vision', 'aprs_vision.*')),
)
