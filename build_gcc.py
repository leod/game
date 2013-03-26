import os
from fabricate import *

target = 'game'
cflags = '-Wall -Wextra -g -std=c++11'.split()

src_dir = 'src'
include_dirs = ['src', 'lib', 'lib/SFML/include', 'lib/SFML/extlibs/headers',
                'lib/enet/include']
lib_dirs = ['lib/SFML/lib', 'lib/enet/.libs']
defines = ['SFML_STATIC', 'GLEW_STATIC', 'WIN32', 'USING_GCC']
libs = ['glew', 'sfml-graphics-s', 'sfml-window-s', 'sfml-system-s',
        'glu32', 'enet', 'ws2_32', 'winmm', 'opengl32', 'jpeg', 'gdi32']

packages = [
    ('core', ['Component', 'Entity', 'EntityManager', 'Event', 'Log',
              'System', 'Tasks',]),
    ('graphics', ['MapRenderer', 'RenderCube', 'RenderSystem',
                  'VisionSystem',]),
    ('input', ['ClockTimeSource', 'InputSource', 'SFMLInputSource',
               'TimeSource',]),
    ('net', ['Definitions', 'ENetReceiver', 'Event', 'EventQueue', 
             'NetComponent', 'NetStateStore', 'NetSystem',]),
    ('map', ['Map',]),
    ('math', ['Intersection',]),
    ('opengl', ['Buffer', 'Framebuffer', 'Error', 'Program', 'ProgramManager',
                'Shader', 'Texture', 'TextureManager',]),
    ('physics', ['PhysicsSystem',]),
    ('world', ['EventTypes', 'LocalPlayerInputComponent', 'PhysicsNetState',
               'PlayerInput', 'PlayerInputComponent', 'PlayerInputSource',
               'ProjectileSystem', 'TickSystem',]),
    ('util', ['Backtrace', 'BitStream', 'Print', 'Profiling',]),
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
    after();
    run('g++',
        '-static',
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
