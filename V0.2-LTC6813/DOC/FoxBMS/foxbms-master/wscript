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

import os
import errno
import sys
import datetime
import logging
import posixpath
import re
import yaml
import glob

from waflib import Utils, Options, Errors
from waflib import Task, TaskGen
from waflib.Tools.compiler_c import c_compiler

__version__ = '1.5.2'
__date__ = '2017-11-29'
__updated__ = '2018-08-15'

out = 'build'
variants = ['primary', 'secondary', 'libs']
from waflib.Build import BuildContext, CleanContext, ListContext, StepContext  # noqa: E402
for x in variants:
    for y in (BuildContext,
              CleanContext,
              ListContext,
              StepContext):
        name = y.__name__.replace('Context', '').lower()

        class tmp_1(y):
            if name == 'build':
                __doc__ = '''executes the {} of {}'''.format(name, x)
            elif name == 'clean':
                __doc__ = '''cleans the project {}'''.format(x)
            elif name == 'list':
                __doc__ = '''lists the targets to execute for {}'''.format(x)
            elif name == 'step':
                __doc__ = '''executes tasks in a step-by-step fashion, for \
debugging of {}'''.format(x)
            cmd = name + '_' + x
            variant = x

    dox = 'doxygen'

    class tmp_2(BuildContext):
        __doc__ = '''creates the {} documentation of {}'''.format(dox, x)
        cmd = dox + '_' + x
        fun = dox
        variant = x


def options(opt):
    opt.load('compiler_c')
    opt.load(['doxygen', 'sphinx_build', 'cpplint', 'flake8'],
             tooldir=os.path.join('tools', 'waftools'))
    opt.add_option('-t', '--target', action='store', default='debug',
                   help='build target: debug (default)/release', dest='target')
    opt.add_option('-l', '--libs', action='store', default='',
                   help='name of the library to be used')
    for k in ('--keep',
              '--targets',
              '--out',
              '--top',
              '--prefix',
              '--destdir',
              '--bindir',
              '--libdir',
              '--msvc_version',
              '--msvc_targets',
              '--no-msvc-lazy',
              '--zones',
              '--force',
              '--check-c-compiler'):
        option = opt.parser.get_option(k)
        if option:
            opt.parser.remove_option(k)

    mctx = waflib.Context.classes
    mctx.remove(waflib.Build.InstallContext)
    mctx.remove(waflib.Build.UninstallContext)


def configure(conf):
    # Setup the whole toolchain (compiler, interpreter etc.)
    print('Compiler toolchain:')
    pref = 'arm-none-eabi-'  # prefix for all gcc related tools
    exe_extension = ''
    if sys.platform.startswith('win'):
        exe_extension = '.exe'
    conf.env.CC = pref + 'gcc' + exe_extension
    conf.env.AR = pref + 'ar' + exe_extension
    conf.env.LINK_CC = pref + 'g++' + exe_extension
    gcc_tools = 'cpp ranlib as strip objcopy objdump size gdb'.split()
    for k in reversed(sorted(gcc_tools, key=len)):
        conf.find_program(pref + k, var=k.upper(), mandatory=True)
    for key in c_compiler:  # force only using gcc
        c_compiler[key] = ['gcc']
    conf.load('compiler_c')
    conf.load(['doxygen', 'sphinx_build', 'cpplint', 'flake8'])
    print('General tools:')
    conf.find_program('python', var='PYTHON', mandatory=True)
    conf.find_program('git', mandatory=False)

    # define configuration files etc.
    # parsing the version info based on the general documentation
    conf.env.sphinx_doc_dir = os.path.join('documentation', 'sphinx')
    to_posix = (conf.env.sphinx_doc_dir).split(os.sep)
    conf.env.sphinx_doc_dir_posix = posixpath.join(*(to_posix))
    conf.env.sphinx_conf_path = os.path.join(conf.env.sphinx_doc_dir,
                                             'conf.py')
    version_info_file = os.path.join(conf.env.sphinx_doc_dir, 'macros.rst')
    with open(version_info_file, 'r', encoding='UTF-8') as f:
        txt = f.read()
    rgx = r'\.\.[ ]\|version\|[ ]replace::[ ]``(\d{1,}\.\d{1,}\.\d{1,})``'
    tmp_version = re.search(rgx, txt)
    try:
        conf.env.version = tmp_version.group(1)
    except AttributeError as err:
        err_msg = 'Could not find a version info in {}.\
'.format(version_info_file)
        conf.fatal(err_msg)

    conf.env.appname = 'foxbms'
    conf.env.appname_prefix = conf.env.appname
    conf.env.vendor = 'Fraunhofer IISB'
    conf.env.version_primary = conf.env.version
    conf.env.version_secondary = conf.env.version

    # setup the scripts etc. for the checksum process
    conf.env.checksum_tools = os.path.join('tools', 'checksum')
    conf.env.chksum_script = os.path.abspath(os.path.join(conf.env.checksum_tools, 'chksum.py'))
    conf.env.writeback_script = os.path.abspath(os.path.join(conf.env.checksum_tools, 'writeback.py'))
    conf.env.cs_out_dir = os.path.normpath('chk5um')
    conf.env.cs_out_file = os.path.join(conf.env.cs_out_dir, 'chksum.yml')

    # Setup the compiler and link flags
    with open('compiler-flags.yml', 'r') as stream:
        try:
            compiler_flags = yaml.load(stream)
        except yaml.YAMLError as exc:
            conf.fatal(exc)
    conf.env.CFLAGS = compiler_flags['CFLAGS']
    conf.env.linkflags = compiler_flags['LINKFLAGS']

    # get HAL version and floating point version based on compiler define and
    # check if cpu and floating point version  are fitting together
    cpu = None
    floating_point_version = None
    for _cflag in conf.env.CFLAGS:
        if 'mcpu' in _cflag:
            cdef, cpu = _cflag.split('=')
        if 'mfpu' in _cflag:
            cdef, floating_point_version = _cflag.split('=')

    if not cpu:
        logging.error('Error: Could not find \'-mcpu\' in compiler flags')
        sys.exit(1)
    if not floating_point_version:
        logging.error('Error: Floating point version not specified')
        sys.exit(1)

    if cpu == 'cortex-m4':
        conf.env.CPU_MAJOR = 'STM32F4xx'
        if floating_point_version != 'fpv4-sp-d16':
            logging.error('Error: floating point unit flag not compatible with cpu')
            sys.exit(1)
    else:
        logging.error('\'%s\' is not a valid cpu version', cpu)
        sys.exit(1)

    # Setup startup and linker script
    if conf.env.CPU_MAJOR == 'STM32F4xx':
        conf.env.ldscript_filename = 'STM32F429ZIT6_FLASH.ld'
        conf.env.startupscript_filename = 'startup_stm32f429xx.S'

    utcnow = datetime.datetime.utcnow()
    utcnow = ''.join(utcnow.isoformat('-').split('.')
                     [0].replace(':', '-').split('-'))
    conf.env.timestamp = utcnow

    conf.define('BUILD_APPNAME_PREFIX', conf.env.appname_prefix)
    for x in variants:
        conf.define(('BUILD_APPNAME_{}'.format(x)).upper(),
                    '{}_{}'.format(conf.env.appname_prefix, x)[:14],
                    comment='Define is trimmed to max. 14 characters'.format(x))
    conf.define('BUILD_VERSION_PRIMARY', conf.env.version_primary)
    conf.define('BUILD_VERSION_SECONDARY', conf.env.version_secondary)

    conf.env.target = conf.options.target
    conf.env.EXT_CC += ['.S']

    env_debug = conf.env.derive()
    env_debug.detach()
    env_release = conf.env.derive()
    env_release.detach()

    # configuration for debug
    conf.setenv('debug', env_debug)
    conf.define('RELEASE', 1)
    conf.undefine('DEBUG')
    conf.env.CFLAGS += ['-g', '-O0']

    # configuration for release
    conf.setenv('release', env_release)
    conf.env.CFLAGS += ['-O2']

    if conf.options.target == 'release':
        conf.setenv('', env_release)
    else:
        conf.setenv('', env_debug)

    env_release.store(os.path.join(out, 'env-store.log'))

    config_dir = 'config'
    try:
        os.makedirs(os.path.join(out, config_dir))
    except OSError as e:
        if e.errno != errno.EEXIST:
            raise

    header_file_name = conf.env.appname_prefix + 'config.h'
    header_file_path = os.path.join(config_dir, header_file_name),
    def_guard = header_file_name.upper().replace('.H', '_H_')
    conf.write_config_header(header_file_path, guard=def_guard)
    print('---')
    print('Vendor:              {}'.format(conf.env.vendor))
    print('Appname prefix:      {}'.format(conf.env.appname_prefix))
    print('Applications:        {}'.format(', '.join(variants)))
    print('Version primary:     {}'.format(conf.env.version_primary))
    print('Version secondary:   {}'.format(conf.env.version_secondary))
    print('---')
    print('Config header:       {}'.format(conf.env.cfg_files))
    print('Build configuration: {}'.format(conf.env.target))
    print('---')
    with open(os.path.join(out, 'linkflags.log'), 'w') as f:
        f.write('\n'.join(conf.env.linkflags) + '\n')
    with open(os.path.join(out, 'cflags.log'), 'w') as f:
        f.write('\n'.join(conf.env.CFLAGS) + '\n')

    conf.path.get_bld().make_node('lib').mkdir()
    x = conf.path.get_bld().find_node('lib')
    conf.env.LIBPATH_USER = x.abspath()
    print('Library directory:   {}'.format(x, x.path_from(conf.path)))

    conf.path.get_bld().make_node('include').mkdir()
    x = conf.path.get_bld().find_node('include')
    conf.env.INCLUDES_USER = x.abspath()
    print('Include directory:   {}'.format(x, x.path_from(conf.path)))

    if conf.options.libs:
        conf.env.LIB_USER = conf.options.libs
        print('Using library:       {}'.format(conf.options.libs))
    else:
        conf.env.LIB_USER = None


def build(bld):
    import sys
    import logging
    from waflib import Logs
    if not bld.variant:
        bld.fatal('A {} variant must be specified, run \'{} {} --help\'\
'.format(bld.cmd, sys.executable, sys.argv[0]))

    log_file = os.path.join(out, 'build_' + bld.variant + '.log')
    bld.logger = Logs.make_logger(log_file, out)
    hdlr = logging.StreamHandler(sys.stdout)
    formatter = logging.Formatter('%(message)s')
    hdlr.setFormatter(formatter)
    bld.logger.addHandler(hdlr)

    bld.env.__sw_dir = os.path.normpath('embedded-software')
    if bld.variant == 'libs':
        src_dir = os.path.normpath('{}'.format(bld.variant))
    else:
        src_dir = os.path.normpath('mcu-{}'.format(bld.variant))
        ldscript = os.path.join(bld.env.__sw_dir, src_dir, 'src', bld.env.ldscript_filename)
        bld.env.ldscript = os.path.join(bld.srcnode.abspath(), ldscript)
        bld.env.stscript = bld.env.startupscript_filename
        bld.env.checksum_config_rel_path = os.path.join('tools', 'checksum', 'checksum_conf.yml')
        bld.env.checksum_config = os.path.abspath(bld.env.checksum_config_rel_path)
        bld.env.__bld_project = src_dir

        bld.env.__bld_common = os.path.normpath('mcu-common')

        bld.env.__inc_FreeRTOS = ' '.join([
            os.path.join(bld.top_dir, bld.env.__sw_dir, 'mcu-freertos', 'Source'),
            os.path.join(bld.top_dir, bld.env.__sw_dir, 'mcu-freertos', 'Source', 'include'),
            os.path.join(bld.top_dir, bld.env.__sw_dir, 'mcu-freertos', 'Source', 'portable', 'GCC', 'ARM_CM4F')])
        bld.env.__inc_hal = ' '.join([
            os.path.join(bld.top_dir, bld.env.__sw_dir, 'mcu-hal', 'CMSIS', 'Device', 'ST', bld.env.CPU_MAJOR, 'Include'),
            os.path.join(bld.top_dir, bld.env.__sw_dir, 'mcu-hal', 'CMSIS', 'Include'),
            os.path.join(bld.top_dir, bld.env.__sw_dir, 'mcu-hal', bld.env.CPU_MAJOR + '_HAL_Driver', 'Inc'),
            os.path.join(bld.top_dir, bld.env.__sw_dir, 'mcu-hal', bld.env.CPU_MAJOR + '_HAL_Driver', 'Inc', 'Legacy')])
        t = os.path.dirname(bld.env.cfg_files[0])
        bld.env.append_value('INCLUDES', t)
    bld.recurse(os.path.join(bld.env.__sw_dir, src_dir))
    bld.add_post_fun(size)


def size(bld):
    base_cmd = '{} --format=berkley'.format(bld.env.SIZE[0])
    print('Running: \'{}\' on all binaries.'.format(base_cmd))
    out_pref = out + '/' + bld.variant + '/'
    _out = '\n'
    for _f in ['**/*.elf', '**/*.a', '**/*.o']:
        for filename in glob.glob(out_pref + _f, recursive=True):
            cmd = '{} {}'.format(base_cmd, os.path.abspath(filename))
            _std_out, _std_err = bld.cmd_and_log(cmd, output=waflib.Context.BOTH, quiet=waflib.Context.STDOUT)
            _out += '{}\n'.format(cmd)
            if _std_out:
                _out += '\n{}'.format(_std_out)
    size_log_file = os.path.join(bld.bldnode.abspath(), 'size_' + bld.variant + '.log')
    with open(size_log_file, 'w') as f:
        f.write(_out)


def clean_all(bld):
    """cleans all parts of the project"""
    from waflib import Options
    commands_after = Options.commands
    Options.commands = ['clean_primary', 'clean_secondary', 'clean_libs']
    Options.commands += commands_after


def build_all(bld):
    """builds all parts of the project (binaries and documentation)"""
    from waflib import Options
    commands_after = Options.commands
    Options.commands = ['build_libs', 'build_primary', 'build_secondary', 'doxygen_primary', 'doxygen_secondary', 'sphinx']
    Options.commands += commands_after


def dist(conf):
    conf.base_name = 'foxbms'
    conf.algo = 'tar.gz'
    conf.excl = out
    conf.excl += ' .ws **/tools/waf*.*.**-* .lock-*'
    conf.excl += ' **/.git **/.gitignore **/.gitattributes '
    conf.excl += ' **/*.tar.bz2 **/*.tar.gz **/*.pyc '


def distcheck_cmd(self):
    import shlex
    cfg = []
    if Options.options.distcheck_args:
        cfg = shlex.split(Options.options.distcheck_args)
    else:
        cfg = [x for x in sys.argv if x.startswith('-')]
    cmd = [sys.executable, sys.argv[0], 'configure', 'build_primary', 'build_secondary', 'doxygen_primary', 'doxygen_secondary', 'sphinx'] + cfg
    return cmd


def check_cmd(self):
    import tarfile
    with tarfile.open(self.get_arch_name())as t:
        for x in t:
            t.extract(x)
    cmd = self.make_distcheck_cmd()
    ret = Utils.subprocess.Popen(cmd, cwd=self.get_base_name()).wait()
    if ret:
        raise Errors.WafError('distcheck failed with code % r' % ret)


def distcheck(conf):
    """creates tar.bz form the source directory and tries to run a build"""
    from waflib import Scripting
    Scripting.DistCheck.make_distcheck_cmd = distcheck_cmd
    Scripting.DistCheck.check = check_cmd
    conf.base_name = 'foxbms'
    conf.excl = out
    conf.excl += ' .ws **/tools/waf*.*.**-* .lock-*'
    conf.excl += ' **/.git **/.gitignore **/.gitattributes '
    conf.excl += ' **/*.tar.bz2 **/*.tar.gz **/*.pyc '


class tsk_chksum(Task.Task):
    always_run = True
    after = ['tsk_binflashheadergen', 'tsk_binflashgen']
    calculate_checksum = '${PYTHON} ${chksum_script} ${checksum_config} ${bld.variant} -csof=${cs_out_file} -bf=${SRC[0].relpath()} -bfh=${SRC[1].relpath()}'
    writeback_command = '${PYTHON} ${writeback_script} --conffile ${cs_out_file} --elffile ${SRC[2].relpath()} --tool ${OBJDUMP}'
    run_str = (calculate_checksum, writeback_command)
    color = 'RED'


@TaskGen.feature('chksum')
@TaskGen.before('add_hexgen_taskk')
@TaskGen.after('apply_link', 'add_bingen_task')
def add_chksum_task(self):
    try:
        link_task = self.link_task
        binflashgen = self.binflashgen_task
        binflashheadergen = self.binflashheadergen_task
    except AttributeError:
        return
    self.chksum_task = self.create_task('tsk_chksum', src=[binflashgen.outputs[0], binflashheadergen.outputs[0], link_task.outputs[0]])


def doxygen(bld):
    import sys
    import logging
    from waflib import Logs

    if not bld.variant:
        bld.fatal('A build variant must be specified, run \'{} {} --help\'\
'.format(sys.executable, sys.argv[0]))

    if not bld.env.DOXYGEN:
        bld.fatal('Doxygen was not configured. Run \'{} {} --help\'\
'.format(sys.executable, sys.argv[0]))

    _docbuilddir = os.path.normpath(bld.bldnode.abspath())
    doxygen_conf_dir = os.path.join('documentation', 'doxygen')
    os.makedirs(_docbuilddir, exist_ok=True)
    conf_file = 'doxygen-{}.conf'.format(bld.variant)
    doxygenconf = os.path.join(doxygen_conf_dir, conf_file)

    log_file = os.path.join(bld.bldnode.abspath(), 'doxygen_' +
                            bld.variant + '.log')
    bld.logger = Logs.make_logger(log_file, out)
    hdlr = logging.StreamHandler(sys.stdout)
    formatter = logging.Formatter('%(message)s')
    hdlr.setFormatter(formatter)
    bld.logger.addHandler(hdlr)

    bld(features='doxygen', doxyfile=doxygenconf)


def flake8(bld):
    bld(features='flake8')


def cpplint(bld):
    from waflib import Logs
    import glob
    with open('cpplint.yml', 'r') as stream:
        try:
            cpplint_conf = yaml.load(stream)
        except yaml.YAMLError as exc:
            bld.fatal(exc)
    files_include = cpplint_conf['files']['include']
    files_exclude = cpplint_conf['files']['exclude']
    files_exclude = tuple(files_exclude)
    output_format = cpplint_conf['output']
    linelength = cpplint_conf['linelength']
    filters = cpplint_conf['filter']
    filters = ','.join(filters)
    to_lint_files = []
    files_include = [os.path.join(bld.top_dir, x) for x in files_include]
    for x in files_include:
        for filename in glob.iglob(x, recursive=True):
            if filename.endswith(files_exclude):
                continue
            else:
                to_lint_files.append(filename)
    cmd = [Utils.subst_vars('${CPPLINT}', bld.env)]
    cmd += ['--output={}'.format(output_format)]
    cmd += ['--linelength={}'.format(linelength)]
    cmd += ['--filter=' + filters]
    err = False
    err_msgs = 'Linting errors are listed below this line\n'
    err_msgs = '------------------------------------------------------------\n'
    err_msgs += 'Error messages are:\n'
    for f in to_lint_files:
        _cmd = cmd + [f]
        Logs.info(' '.join(_cmd))
        proc = Utils.subprocess.Popen(_cmd, stdout=Utils.subprocess.PIPE, stderr=Utils.subprocess.PIPE)
        std_out, std_err = proc.communicate()
        std_out, std_err = std_out.decode(), std_err.decode()
        if std_out:
            print(std_out)
        if std_err:
            print(std_err)
        if proc.returncode:
            err = True
            err_msgs = err_msgs + std_err + '\n'
    if err:
        Logs.error(err_msgs)
        bld.fatal('There are linting errors.')


def sphinx(bld):
    import sys
    import logging
    from waflib import Logs
    if not bld.env.SPHINX_BUILD:
        bld.fatal('ERROR: cannot build documentation (\'sphinx-build\' is not \
found in PATH)')
    log_file = os.path.join(bld.bldnode.abspath(), 'sphinx.log')
    bld.logger = Logs.make_logger(log_file, out)
    hdlr = logging.StreamHandler(sys.stdout)
    formatter = logging.Formatter('%(message)s')
    hdlr.setFormatter(formatter)
    bld.logger.addHandler(hdlr)
    rst_srcs = '{}/**/*.rst'.format(bld.env.sphinx_doc_dir_posix)
    bld(features='sphinx',
        outdir='documentation',
        source=bld.path.ant_glob(rst_srcs),
        config=bld.env.sphinx_conf_path,
        VERSION=bld.env.version,
        RELEASE=bld.env.version)


class strip(Task.Task):
    run_str = '${STRIP} ${SRC}'
    color = 'BLUE'


@TaskGen.feature('strip')
@TaskGen.after('apply_link')
def add_strip_task(self):
    try:
        link_task = self.link_task
    except AttributeError:
        return
    self.create_task('strip', link_task.outputs[0])


class hexgen(Task.Task):
    always_run = True
    after = ['tsk_chksum']
    run_str = '${OBJCOPY} -O ihex ${SRC} ${TGT}'
    color = 'CYAN'


@TaskGen.feature('hexgen')
@TaskGen.after('add_chksum_task')
def add_hexgen_task(self):
    try:
        link_task = self.link_task
    except AttributeError:
        return
    self.hexgen = self.create_task('hexgen', src=link_task.outputs[0], tgt=link_task.outputs[0].change_ext('.hex'))


class tsk_binflashheaderpatch(Task.Task):
    always_run = True
    after = ['tsk_chksum']
    run_str = '${OBJCOPY} -j .flashheader -O binary ${SRC} ${TGT}'
    color = 'RED'


class tsk_binflashheadergen(Task.Task):
    always_run = True
    run_str = '${OBJCOPY} -j .flashheader -O binary ${SRC} ${TGT}'
    color = 'RED'


class tsk_binflashgen(Task.Task):
    always_run = True
    run_str = '${OBJCOPY} -R .ext_sdramsect_bss -R .bkp_ramsect -R .flashheader -O binary ${SRC} ${TGT}'
    color = 'RED'


@TaskGen.feature('bingen')
@TaskGen.before('add_chksum_task')
@TaskGen.after('apply_link')
def add_bingen_task(self):
    try:
        link_task = self.link_task
    except AttributeError:
        return
    self.binflashgen_task = self.create_task('tsk_binflashgen', src=link_task.outputs[0], tgt=link_task.outputs[0].change_ext('_flash.bin'))
    self.binflashheadergen_task = self.create_task('tsk_binflashheadergen', src=link_task.outputs[0], tgt=link_task.outputs[0].change_ext('_flashheader.bin'))
    self.binflashheaderpatch_task = self.create_task('tsk_binflashheaderpatch', src=[link_task.outputs[0], self.binflashheadergen_task.outputs[0]])


import waflib.Tools.asm    # noqa: E402 import before redefining
from waflib.TaskGen import extension  # noqa: E402


class Sasm(Task.Task):
    color = 'BLUE'
    run_str = '${CC} ${CFLAGS} ${CPPPATH_ST:INCPATHS} -DHSE_VALUE=8000000 -MMD -MP -MT${TGT} -c -x assembler -o ${TGT} ${SRC}'


@extension('.S')
def asm_hook(self, node):
    name = 'Sasm'
    out = node.change_ext('.o')
    task = self.create_task(name, node, out)
    try:
        self.compiled_tasks.append(task)
    except AttributeError:
        self.compiled_tasks = [task]
    return task
