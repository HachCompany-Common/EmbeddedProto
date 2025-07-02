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
    print("Debug: Entering get_current_branch()")
    try:
        # Try the normal git command first
        print("Debug: Running git rev-parse --abbrev-ref HEAD")
        result = subprocess.run(
            ["git", "rev-parse", "--abbrev-ref", "HEAD"],
            capture_output=True,
            text=True,
            check=True
        )
        branch = result.stdout.strip()
        print(f"Debug: git rev-parse result: '{branch}'")
        
        # In GitHub Actions, this might return "HEAD" for detached HEAD state
        if branch == "HEAD" and os.getenv('GITHUB_ACTIONS') == 'true':
            print("Debug: Detected HEAD in GitHub Actions, using GITHUB_REF")
            # Use GITHUB_REF to determine the branch or tag
            github_ref = os.getenv('GITHUB_REF', '')
            print(f"Debug: GITHUB_REF = '{github_ref}'")
            
            if github_ref.startswith('refs/heads/'):
                branch_name = github_ref.replace('refs/heads/', '')
                print(f"Debug: Extracted branch name from GITHUB_REF: '{branch_name}'")
                return branch_name
            elif github_ref.startswith('refs/tags/'):
                print("Debug: GITHUB_REF is a tag, assuming master branch")
                return 'master'  # Assume tags are created from master
            else:
                print("Debug: GITHUB_REF format not recognized, defaulting to develop")
                return 'develop'  # Default
        
        print(f"Debug: Returning branch name: '{branch}'")
        return branch
    except subprocess.CalledProcessError as e:
        print(f"Debug: Error in git rev-parse: {e}")
        # If git command fails, try to use GitHub environment variables
        if os.getenv('GITHUB_ACTIONS') == 'true':
            github_ref = os.getenv('GITHUB_REF', '')
            print(f"Debug: In GitHub Actions, GITHUB_REF = '{github_ref}'")
            
            if github_ref.startswith('refs/heads/'):
                branch_name = github_ref.replace('refs/heads/', '')
                print(f"Debug: Extracted branch name from GITHUB_REF: '{branch_name}'")
                return branch_name
            elif github_ref.startswith('refs/tags/'):
                print("Debug: GITHUB_REF is a tag, assuming master branch")
                return 'master'  # Assume tags are created from master
        
        # Default fallback
        print("Debug: No branch information found, defaulting to develop")
        return 'develop'


def get_version():
    try:
        version_file = 'EmbeddedProto/version.json'
        build_number = os.getenv('GITHUB_RUN_NUMBER', '0')
        print(f"Debug: GITHUB_RUN_NUMBER = '{build_number}'")
        print(f"Debug: GITHUB_REF = '{os.getenv('GITHUB_REF', 'not set')}'")
        print(f"Debug: GITHUB_SHA = '{os.getenv('GITHUB_SHA', 'not set')}'")

        try:
            with open(version_file, 'r') as f:
                version_data = json.load(f)
            base_version = version_data.get('version', '0.0.0')
            print(f"Debug: Base version from version.json: '{base_version}'")
        except (FileNotFoundError, json.JSONDecodeError) as e:
            print(f"Debug: Error reading version.json: {e}")
            # Fallback if version file is missing or invalid
            base_version = '0.0.0'

        branch_name = get_current_branch()
        print(f"Debug: Detected branch name: '{branch_name}'")

        if branch_name == "master":
            # Final release version (no suffix)
            full_version = base_version
            print(f"Debug: Using master version: '{full_version}'")

        elif re.fullmatch(r'release/\d+\.\d+\.\d+', branch_name):
            # Release candidate version
            build_number = os.getenv('GITHUB_RUN_NUMBER', '0')
            print(f"Debug: Detected release branch, using GITHUB_RUN_NUMBER '{build_number}' for rc suffix")
            full_version = f"{base_version}rc{build_number}"
            print(f"Debug: Using release candidate version: '{full_version}'")

        else:
            # Development or feature branch
            full_version = f"{base_version}.dev{build_number}"
            print(f"Debug: Using development version: '{full_version}'")

        return full_version
    except Exception as e:
        # Last resort fallback in case of any unexpected errors
        print(f"Warning: Error determining version: {e}")
        return '0.0.0.dev0'


setup(
    cmdclass={
        "editable_wheel": EditableWheel,
        "sdist": Sdist,
    },
    version=get_version(),
)


####################################################################################

if __name__ == "__main__":
    get_version()
