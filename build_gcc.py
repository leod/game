import os
from fabricate import *

target = 'game'
cflags = '-Wall -g -std=c++11'.split()

src_dir = 'src'
include_dirs = ['src', 'lib', 'lib/SFML/include', 'lib/SFML/extlibs/headers',
                'lib/enet/include']
lib_dirs = ['lib/SFML/lib', 'lib/enet/.libs']
defines = ['SFML_STATIC', 'GLEW_STATIC']
libs = ['sfml-graphics-s', 'sfml-window-s', 'sfml-system-s', 'glu32',
        'opengl32', 'enet']

packages = [
    ('client', ['Main',]),
    ('core', ['Component', 'EntityRegistry', 'System', 'Tasks',]),
    ('graphics', ['RenderCube', 'RenderSystem',]),
    ('input', ['ClockTimeSource', 'InputSource', 'SFMLInputSource',
               'TimeSource',]),
    ('opengl', ['Buffer', 'Error', 'Program', 'ProgramManager', 'Shader',
                'Texture', 'TextureManager',]),
    ('world', ['PlayerInputComponent', 'PlayerInputSource', 'TickSystem',]),
]

def build():
    compile()
    link()

def join_flags(flag, params):
    return [flag + param for param in params]

def compile(build_dir=os.path.join('build', 'gcc'), flags=None):
    for package, sources in packages:
        for source in sources:
            run('g++',
                join_flags('-I', include_dirs),
                join_flags('-D', defines),
                '-c', os.path.join(src_dir, package, source + '.cpp'),
                '-o', os.path.join(build_dir, package + '_'  + source + '.o'),
                cflags, flags)

def link(build_dir=os.path.join('build', 'gcc'), flags=None):
    objects = [os.path.join(build_dir, package + '_' + source + '.o')
               for (package, sources) in packages
               for source in sources]
    after();
    run('g++',
        join_flags('-L', lib_dirs), 
        objects, join_flags('-l', libs), '-o', target, flags)

def check():
    return int(outofdate(build))

def clean():
    autoclean()

def rebuild():
    clean()
    build()

main()
