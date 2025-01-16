from setuptools import find_packages, setup
import os
from glob import glob

package_name = 'akb_application'

setup(
    name=package_name,
    version='0.0.0',
    packages=find_packages(exclude=['test']),
    data_files=[
        ('share/ament_index/resource_index/packages',
            ['resource/' + package_name]),
        ('share/' + package_name, ['package.xml']),
        (os.path.join('share', package_name, 'launch'), glob('launch/*.launch.py'))
    ],
    install_requires=['setuptools'],
    zip_safe=True,
    maintainer='justin',
    maintainer_email='jalbrecht23@gmail.com',
    description='TODO: Package description',
    license='NIST',
    tests_require=['pytest'],
    entry_points={
        'console_scripts': [
            'akb_application = akb_application.akb_application:main',
            'test_server = akb_application.dummy_nodes:main'
        ],
    },
)
