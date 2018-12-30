#!/usr/bin/env python
# encoding: utf-8

# inspired by code by Hans-Martin von Gaudecker, 2012

import os
from waflib import Node, Task, TaskGen, Errors, Logs, Build, Utils

class sphinx_build(Task.Task):
    color = 'BLUE'
    run_str = '${SPHINX_BUILD} -c ${CONFDIR} -D ${VERSION} -D ${RELEASE} -D graphviz_dot=${dot} -q -b ${BUILDERNAME} -d ${DOCTREEDIR} ${SRCDIR} ${OUTDIR}'

    def __str__(self):
        env = self.env
        src_str = ' '.join([a.path_from(a.ctx.launch_node()) for a in self.inputs])
        tgt_str = ' '.join([a.path_from(a.ctx.launch_node()) for a in self.outputs])
        if self.outputs: sep = ' -> '
        else: sep = ''
        return'%s [%s]: %s%s%s\n'%(self.__class__.__name__.replace('_task',''),
                                   self.env['BUILDERNAME'], src_str, sep, tgt_str)

@TaskGen.extension('.py', '.rst')
def sig_hook(self, node):
    node.sig=Utils.h_file(node.abspath())

@TaskGen.feature("sphinx")
@TaskGen.before_method("process_source")
def apply_sphinx(self):
    """Set up the task generator with a Sphinx instance and create a task."""

    inputs = []
    for i in Utils.to_list(self.source):
        if not isinstance(i, Node.Node):
            node = self.path.find_node(node)
        else:
            node = i
        if not node:
            raise ValueError('[%s] file not found' % i)
        inputs.append(node)

    task = self.create_task('sphinx_build', inputs)

    conf = self.path.find_node(self.config)
    task.inputs.append(conf)

    confdir = conf.parent.abspath()
    buildername = getattr(self, "builder", "html")
    srcdir = getattr(self, "srcdir", confdir)
    outdir = self.path.find_or_declare(getattr(self, "outdir", buildername)).get_bld()
    doctreedir = getattr(self, "doctreedir", os.path.join(outdir.abspath(), ".doctrees"))

    task.env['BUILDERNAME'] = buildername
    task.env['SRCDIR'] = srcdir
    task.env['CONFDIR'] = confdir
    task.env['DOCTREEDIR'] = doctreedir
    task.env['OUTDIR'] = outdir.abspath()
    task.env['VERSION'] = "version=%s" % self.VERSION
    task.env['RELEASE'] = "release=%s" % self.VERSION

    import imp
    confData = imp.load_source('sphinx_conf', conf.abspath())

    if buildername == "man":
        for i in confData.man_pages:
            target = outdir.find_or_declare('%s.%d' % (i[1], i[4]))
            task.outputs.append(target)

            if self.install_path:
                self.bld.install_files("%s/man%d/" % (self.install_path, i[4]), target)
    else:
        task.outputs.append(outdir)

def configure(conf):
    print('Sphinx documentation tools:')
    conf.find_program('sphinx-build', var='SPHINX_BUILD', mandatory=False)
    conf.find_program('dot', var='dot', mandatory=True) 

# sphinx docs
from waflib.Build import BuildContext, CleanContext
class sphinx(BuildContext):
    __doc__ = '''creates the sphinx documentation of the project'''  # foxbms
    cmd = "sphinx"
    fun = "sphinx"
