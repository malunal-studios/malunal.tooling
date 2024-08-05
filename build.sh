CONFIGURE=0
RELEASE=0
INSTALL=0
THREADS=1
EXAMPLE=0


function usage {
    echo "Usage: $0 [-c] [-r] [-i] [-t THREADS] [-e] [-h]"
    echo "  -c:         Whether to configure the project or just build it."
    echo "  -r:         Whether to build a release or debug version of the project."
    echo "  -i:         Whether to install the project."
    echo "  -t THREADS: The number of threads to use to build the project."
    echo "  -e:         Wether to build the example for the project."
    exit $1
}


while getopts "crit:eh" option
do
    case $option in
    c) CONFIGURE=1;;
    r) RELEASE=1;;
    i) INSTALL=1;;
    t) THREADS=${OPTARG};;
    e) EXAMPLE=1;;
    h) usage 0;;
    *) usage 1;;
    esac
done
shift $((OPTIND-1))


if [ $CONFIGURE -eq 1 ]; then
    BUILD_TYPE="Debug"
    if [ $RELEASE -eq 1 ]; then
        BUILD_TYPE="Release"
    fi

    # Set build type.
    OPTIONS="-DCMAKE_BUILD_TYPE=${BUILD_TYPE}"
    if [ $INSTALL -eq 1 ]; then
        OPTIONS="${OPTIONS} -DSIMULAR_TOOLING_INSTALL=ON"
    fi

    # Build example?
    if [ $EXAMPLE -eq 1 ]; then
        OPTIONS="${OPTIONS} -DSIMULAR_TOOLING_BUILD_EXAMPLE=ON"
    fi

    echo "Configuring project..."
    cmake -G "Unix Makefiles" ${OPTIONS} -S . -B build
fi


echo "Building project..."
cmake --build build -- -j${THREADS}

if [ $INSTALL -eq 1 ]; then
    echo "Installing project..."
    sudo cmake --install build
fi
