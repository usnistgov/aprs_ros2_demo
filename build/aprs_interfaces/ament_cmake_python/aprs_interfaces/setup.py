from setuptools import find_packages
from setuptools import setup

setup(
    name='aprs_interfaces',
    version='1.0.0',
    packages=find_packages(
        include=('aprs_interfaces', 'aprs_interfaces.*')),
)
