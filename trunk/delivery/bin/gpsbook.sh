 #!/bin/bash
appname=`basename $0 | sed s,\.sh$,,`
dirname=`dirname $0`
cd $dirname
LD_LIBRARY_PATH=$dirname/../lib:$dirname/../plugins
export LD_LIBRARY_PATH
$dirname/$appname $*

