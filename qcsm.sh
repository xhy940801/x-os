if [ $# != 1 ] ; then
    echo "usage: $0 module_name"
    exit 1
fi

mkdir $1 && cp Makefile.module.template $1/Makefile
