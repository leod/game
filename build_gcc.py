import os
from fabricate import *

target = 'game'
cflags = '-Wall -Wextra -g -std=c++11'.split()

src_dir = 'src'
include_dirs = ['src', 'lib', 'lib/SFML/include', 'lib/SFML/extlibs/headers',
                'lib/enet/include']
lib_dirs = ['lib/SFML/lib', 'lib/enet/.libs']
defines = ['SFML_STATIC', 'GLEW_STATIC', 'WIN32', 'USING_GCC']

# If you change this order, ld.exe will crash. Have fun.
libs = ['glew', 'sfml-graphics-s', 'sfml-window-s', 'sfml-system-s',
        'sfml-audio-s', 'glu32', 'enet', 'ws2_32', 'winmm', 'opengl32', 'jpeg',
        'gdi32', 'sndfile']

corePackages = [
    ('core', ['Component', 'Entity', 'EntityManager', 'Event', 'Log',
              'System', 'Tasks',]),
    ('graphics', ['GroundRenderer', 'OBJ', 'RenderCube', 'RenderOBJComponent',
                  'RenderMapObjectComponent', 'RenderPlayerComponent',
                  'RenderSystem', 'VisionSystem',]),
    ('input', ['ClockTimeSource', 'InputSource', 'SFMLInputSource',
               'TimeSource',]),
    ('net', ['Definitions', 'ENetReceiver', 'Event', 'EventQueue', 
             'NetComponent', 'NetStateStore', 'NetSystem',]),
    ('math', ['Intersection',]),
    ('opengl', ['Buffer', 'Framebuffer', 'Error', 'Program', 'ProgramManager',
                'Shader', 'Texture', 'TextureManager',]),
    ('sound', ['Sound', 'SoundPlayer',]),
    ('world', ['EventTypes', 'LocalPlayerInputComponent', 'Map',
               'PhysicsNetState', 'PhysicsSystem', 'PlayerInput',
               'PlayerInputComponent', 'PlayerInputSource', 'ProjectileSystem',
               'TickSystem',]),
    ('util', ['Backtrace', 'BitStream', 'Print', 'Profiling',]),
]

clientPackages = [
    ('client', ['Main',]),
]

serverPackages = [
    ('server', ['Main', 'Clients', 'ServerNetSystem',]),
]

def build():
    compile(corePackages + clientPackages + serverPackages)
    link(corePackages + clientPackages, 'client')
    link(corePackages + serverPackages, 'server')

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
