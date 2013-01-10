import os
from fabricate import *

target = 'game'
cflags = '-Wall -Wextra -pedantic -g -std=c++11 -O2'.split()

src_dir = 'src'
include_dirs = ['src', 'lib', 'lib/SFML/include', 'lib/SFML/extlibs/headers',
                'lib/enet/include']
lib_dirs = ['lib/SFML/lib', 'lib/enet/.libs']
defines = ['SFML_STATIC', 'GLEW_STATIC', 'WIN32']
libs = ['sfml-graphics-s', 'sfml-window-s', 'sfml-system-s', 'glu32',
        'opengl32', 'ws2_32', 'winmm']

packages = [
    ('core', ['Component', 'Entity', 'EntityRegistry', 'System', 'Tasks',]),
    ('graphics', ['MapRenderer', 'RenderCube', 'RenderSystem',
                  'VisionSystem',]),
    ('input', ['ClockTimeSource', 'InputSource', 'SFMLInputSource',
               'TimeSource',]),
    ('net', ['MessageHub', 'MessageTypes', 'NetComponent', 'NetStateStore',
             'NetSystem',]),
    ('map', ['Map',]),
    ('math', ['Intersection',]),
    ('opengl', ['Buffer', 'Error', 'Program', 'ProgramManager', 'Shader',
                'Texture', 'TextureManager',]),
    ('world', ['PhysicsNetState', 'PlayerInput', 'PlayerInputComponent',
               'PlayerInputSource', 'TickSystem',]),
    ('util', ['Backtrace', 'BitStream',]),
]

clientPackages = [
    ('client', ['Main',]),
]

serverPackages = [
    ('server', ['Main', 'Clients', 'ServerNetSystem',]),
]

def build():
    compile(packages + clientPackages + serverPackages)
    link(packages + clientPackages, 'client')
    link(packages + serverPackages, 'server')

def join_flags(flag, params):
    return [flag + param for param in params]

def compile(packages, build_dir=os.path.join('build', 'gcc'), flags=None):
    for package, sources in packages:
        for source in sources:
            run('g++',
                join_flags('-I', include_dirs),
                join_flags('-D', defines),
                '-c', os.path.join(src_dir, package, source + '.cpp'),
                '-o', os.path.join(build_dir, package + '_'  + source + '.o'),
                cflags, flags)

def link(packages, target, build_dir=os.path.join('build', 'gcc'), flags=None):
    objects = [os.path.join(build_dir, package + '_' + source + '.o')
               for (package, sources) in packages
               for source in sources]
    # This is a workaround. Linking libenet.a and libwinmm.a directly
    # causes ld.exe to crash.
    bs = join_flags('lib/enet/.libs/', ['callbacks.o', 'compress.o', 'host.o',
                                        'list.o', 'packet.o', 'peer.o',
                                        'protocol.o', 'unix.o', 'win32.o'])
    after();
    run('g++',
        join_flags('-L', lib_dirs), 
        objects, bs, join_flags('-l', libs), '-o', target, flags)

def check():
    return int(outofdate(build))

def clean():
    autoclean()

def rebuild():
    clean()
    build()

main()
