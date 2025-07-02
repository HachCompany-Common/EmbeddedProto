#
# Copyright (C) 2020-2025 Embedded AMS B.V. - All Rights Reserved
#
# This file is part of Embedded Proto.
#
# Embedded Proto is open source software: you can redistribute it and/or
# modify it under the terms of the GNU General Public License as published
# by the Free Software Foundation, version 3 of the license.
#
# Embedded Proto  is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with Embedded Proto. If not, see <https://www.gnu.org/licenses/>.
#
# For commercial and closed source application please visit:
# <https://embeddedproto.com/pricing/>.
#
# Embedded AMS B.V.
# Info:
#   info at EmbeddedProto dot com
#
# Postal address:
#   Atoomweg 2
#   1627 LE, Hoorn
#   the Netherlands
#

from setuptools import find_packages
from setuptools.command.build import build
from setuptools.command.editable_wheel import editable_wheel
from setuptools.command.sdist import sdist
from setuptools import setup
import subprocess
import os
import json
import re


def build_proto():

    command = [
        "python3",
        "-m",
        "grpc_tools.protoc",
        "-I./EmbeddedProto",
        "--python_out=EmbeddedProto",
        "embedded_proto_options.proto",
    ]

    subprocess.run(command, check=True)


class EditableWheel(editable_wheel):
    def run(self):
        build_proto()
        super().run()


class Sdist(sdist):
    def run(self):
        build_proto()
        super().run()

def get_current_branch():
    result = subprocess.run(
        ["git", "rev-parse", "--abbrev-ref", "HEAD"],
        capture_output=True,
        text=True,
        check=True
    )
    return result.stdout.strip()

def get_unique_commit_count(base_branch="main"):
    result = subprocess.run(
        ["git", "rev-list", "--count", f"{base_branch}..HEAD"],
        capture_output=True,
        text=True,
        check=True
    )
    return result.stdout.strip()

def get_version():
    version_file = 'EmbeddedProto/version.json'
    build_number = os.getenv('GITHUB_RUN_NUMBER', '0')

    with open(version_file, 'r') as f:
        version_data = json.load(f)

    base_version = version_data.get('version', '0.0.0')
    branch_name = get_current_branch()

    if branch_name == "master":
        # Final release version (no suffix)
        full_version = base_version

    elif re.fullmatch(r'release/\d+\.\d+\.\d+', branch_name):
        # Release candidate version
        base_branch = 'develop'  # Release candidates are branched of develop.
        rc_number = get_unique_commit_count(base_branch)
        full_version = f"{base_version}rc{rc_number}"

    else:
        # Development or feature branch
        full_version = f"{base_version}.dev{build_number}"

    return full_version


setup(
    cmdclass={
        "editable_wheel": EditableWheel,
        "sdist": Sdist,
    },
    version=get_version(),
)
