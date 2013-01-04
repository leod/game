import os
from fabricate import *

target = 'game'
cflags = '-Wall -Wextra -pedantic -g -std=c++11'.split()

src_dir = 'src'
include_dirs = ['src', 'lib', 'lib/SFML/include', 'lib/SFML/extlibs/headers',
                'lib/enet/include']
lib_dirs = ['lib/SFML/lib', 'lib/enet/.libs']
defines = ['SFML_STATIC', 'GLEW_STATIC', 'WIN32']
libs = ['sfml-graphics-s', 'sfml-window-s', 'sfml-system-s', 'glu32',
        'opengl32', 'ws2_32', 'winmm']

packages = [
    ('client', ['Main',]),
    ('core', ['Component', 'EntityRegistry', 'System', 'Tasks',]),
    ('graphics', ['RenderCube', 'RenderSystem',]),
    ('input', ['ClockTimeSource', 'InputSource', 'SFMLInputSource',
               'TimeSource',]),
    ('net', ['MessageHub', 'NetComponent', 'NetStateStore', 'NetSystem',]),
    ('opengl', ['Buffer', 'Error', 'Program', 'ProgramManager', 'Shader',
                'Texture', 'TextureManager',]),
    ('world', ['PhysicsNetState', 'PlayerInputComponent', 'PlayerInputSource',
               'TickSystem',]),
    ('util', ['BitStream',]),
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
    bs = join_flags('lib/enet/.libs/', ['callbacks.o', 'compress.o', 'host.o',
                                        'list.o', 'packet.o', 'peer.o',
                                        'protocol.o', 'unix.o', 'win32.o'])
    after();
    run('g++',
        join_flags('-L', lib_dirs), 
        '--verbose',
        objects, bs, join_flags('-l', libs), '-o', target, flags)

def check():
    return int(outofdate(build))

def clean():
    autoclean()

def rebuild():
    clean()
    build()

main()
