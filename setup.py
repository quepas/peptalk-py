import os
import sys
import platform
import subprocess

from setuptools import setup, Extension
from setuptools.command.build_ext import build_ext


class CMakeExtension(Extension):
    def __init__(self, name, sourcedir=''):
        Extension.__init__(self, name, sources=[])
        # Reconstruct an absolute path to our cmake module
        self.sourcedir = os.path.abspath(sourcedir)


class CMakeBuild(build_ext):
    def run(self):
        # Check if cmake is installed
        try:
            out = subprocess.check_output(['cmake', '--version'])
        except OSError:
            raise RuntimeError("CMake must be installed to build the following extensions: " +
                               ", ".join(e.name for e in self.extensions))
        # Build each extension from the `ext_modules` field (including our CMakeExtension)
        for ext in self.extensions:
            self.build_extension(ext)

    def build_extension(self, ext):
        # Path to the compiled module (e.g. peptalk_core_impl.cpython3.8*.so)
        extdir = os.path.abspath(os.path.dirname(self.get_ext_fullpath(ext.name)))
        # Check if path ends with the separator
        # Required for auto-detection of auxiliary "native" libs
        if not extdir.endswith(os.path.sep):
            extdir += os.path.sep

        cmake_args = ['-DCMAKE_LIBRARY_OUTPUT_DIRECTORY=' + extdir,
                      '-DPYTHON_EXECUTABLE=' + sys.executable]

        cfg = 'Debug' if self.debug else 'Release'
        build_args = ['--config', cfg]

        if platform.system() == "Windows":
            cmake_args += ['-DCMAKE_LIBRARY_OUTPUT_DIRECTORY_{}={}'.format(cfg.upper(), extdir)]
            if sys.maxsize > 2**32:
                cmake_args += ['-A', 'x64']
            build_args += ['--', '/m']
        else:
            cmake_args += ['-DCMAKE_BUILD_TYPE=' + cfg]
            build_args += ['--', '-j2']

        env = os.environ.copy()
        env['CXXFLAGS'] = '{} -DVERSION_INFO=\\"{}\\"'.format(env.get('CXXFLAGS', ''),
                                                              self.distribution.get_version())
        # Create build directory
        if not os.path.exists(self.build_temp):
            os.makedirs(self.build_temp)
        # Generate build files for the cmake module
        subprocess.check_call(['cmake', ext.sourcedir] + cmake_args, cwd=self.build_temp, env=env)
        # Build the cmake module
        subprocess.check_call(['cmake', '--build', '.'] + build_args, cwd=self.build_temp)

setup(
    name="peptalk",
    version="0.1.0",
    author="quepas",
    author_email="kiepas.patryk@gmail.com",
    description="",
    long_description='',
    url="https://github.com/quepas/peptalk-py.git",
    license="MIT License",
    ext_modules=[CMakeExtension('peptalk_core_impl')],
    cmdclass=dict(build_ext=CMakeBuild),
    zip_safe=False,
    packages=['peptalk']
)

