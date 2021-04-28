#!/bin/sh

if [ -n "$DESTDIR" ] ; then
    case $DESTDIR in
        /*) # ok
            ;;
        *)
            /bin/echo "DESTDIR argument must be absolute... "
            /bin/echo "otherwise python's distutils will bork things."
            exit 1
    esac
fi

echo_and_run() { echo "+ $@" ; "$@" ; }

echo_and_run cd "/home/houlam/yumi_depends_ws/src/yumi/easy_handeye/easy_handeye"

# ensure that Python install destination exists
echo_and_run mkdir -p "$DESTDIR/home/houlam/yumi_depends_ws/install/lib/python2.7/dist-packages"

# Note that PYTHONPATH is pulled from the environment to support installing
# into one location when some dependencies were installed in another
# location, #123.
echo_and_run /usr/bin/env \
    PYTHONPATH="/home/houlam/yumi_depends_ws/install/lib/python2.7/dist-packages:/home/houlam/yumi_depends_ws/build/lib/python2.7/dist-packages:$PYTHONPATH" \
    CATKIN_BINARY_DIR="/home/houlam/yumi_depends_ws/build" \
    "/usr/bin/python2" \
    "/home/houlam/yumi_depends_ws/src/yumi/easy_handeye/easy_handeye/setup.py" \
    egg_info --egg-base /home/houlam/yumi_depends_ws/build/yumi/easy_handeye/easy_handeye \
    build --build-base "/home/houlam/yumi_depends_ws/build/yumi/easy_handeye/easy_handeye" \
    install \
    --root="${DESTDIR-/}" \
    --install-layout=deb --prefix="/home/houlam/yumi_depends_ws/install" --install-scripts="/home/houlam/yumi_depends_ws/install/bin"
