#!/usr/bin/env python
# -*- coding: utf-8 -*-

# @copyright &copy; 2010 - 2018, Fraunhofer-Gesellschaft zur Foerderung der
#   angewandten Forschung e.V. All rights reserved.
#
# BSD 3-Clause License
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions are met:
# 1.  Redistributions of source code must retain the above copyright notice,
#     this list of conditions and the following disclaimer.
# 2.  Redistributions in binary form must reproduce the above copyright notice,
#     this list of conditions and the following disclaimer in the documentation
#     and/or other materials provided with the distribution.
# 3.  Neither the name of the copyright holder nor the names of its
#     contributors may be used to endorse or promote products derived from this
#     software without specific prior written permission.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
# AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
# IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
# ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
# LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
# CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
# SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
# INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
# CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
# ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
# POSSIBILITY OF SUCH DAMAGE.
#
# We kindly request you to use one or more of the following phrases to refer to
# foxBMS in your hardware, software, documentation or advertising materials:
#
# &Prime;This product uses parts of foxBMS&reg;&Prime;
#
# &Prime;This product includes parts of foxBMS&reg;&Prime;
#
# &Prime;This product is derived from foxBMS&reg;&Prime;

from waflib import Node, Task, TaskGen, Errors, Logs, Build, Utils


class flake8_runner(Task.Task):
    color = 'PINK'

    def run(self):
        env = self.env.env or None
        cmd = Utils.subst_vars('${FLAKE_EIGHT} ', self.env)
        proc = Utils.subprocess.Popen(cmd, stdout=Utils.subprocess.PIPE,
                                      stderr=Utils.subprocess.PIPE, env=env,
                                      shell=True)
        out, err = proc.communicate()
        out, err = out.decode(), err.decode()
        if out:
            print(out)
        if err:
            print(err)
        return proc.returncode


@TaskGen.feature('flake8')
def apply_flake8(self):
    """Creates a task to run flake8"""
    self.create_task('flake8_runner')


def configure(conf):
    print('Python Linting tool:')
    conf.find_program('flake8', var='FLAKE_EIGHT', mandatory=True)


from waflib.Build import BuildContext


class flake8(BuildContext):
    __doc__ = '''runs flake8 on the foxBMS repository'''
    cmd = 'flake8'
    fun = 'flake8'
