#!/bin/bash

#usage: runAlgandStats.sh <cfg-path> <video-in> <idealbg> <data-dir>

set -e

IMGW=320
IMGH=240
FPS=5

CFGPATH=`realpath $1`
INPUTVIDEO=`realpath $2`
IDEALBG=`realpath $3`

echo $#
if [ $# -eq 4 ];then #datadir supplied
    if [ ! -d $4 ]; then
	mkdir -p $4
    fi
    DATADIR=`realpath $4`
else
    DATADIR=`mktemp -d`
fi
echo "Datadir: ${DATADIR}"

#dump frames from video: 320x240@5fps
NINPUT=`ls ${DATADIR}/input*.pnm | wc -l`
if [ ${NINPUT} -eq 0 ]; then
    ffmpeg -i ${INPUTVIDEO} -f image2 -s ${IMGW}x${IMGH} -r ${FPS} ${DATADIR}/input%05d.pnm
fi

NIMG=`ls ${DATADIR}/input*.pnm | wc -l`
NROW=$IMGH
NCOL=$IMGW
NCHANNEL=3
RADIUS=10

#run bgfglab with idealbg and FIXED to get ideal fg mask
NIDEALDUMP=`ls ${DATADIR}/ideal.dump*.fgmask.pnm | wc -l`
if [ $NIDEALDUMP -ne $NIMG ]; then
    bgfglab --BGFGlab.Endpoints=default \
	--BGFGlab.Config.ImageProvider.Source=1 \
	--BGFGlab.Config.ImageProvider.LocalSourcePath=${DATADIR}/input%05d.pnm \
	--BGFGlab.Config.BGAlgorithm.Name=Fixed \
	--BGFGlab.Config.BGAlgorithm.Fmt=RGB888 \
	--BGFGlab.Config.BGAlgorithm.InitialImg=${IDEALBG} \
	--BGFGlab.Config.Dump.File=${DATADIR}/ideal.dump%05d \
	--BGFGlab.Config.Dump.Frames=${NIMG} \
	--BGFGlab.Config.Dump.DumpIMG=0 \
	--BGFGlab.Config.Dump.DumpBG=0 \
	--BGFGlab.Config.Dump.DumpFG=1 \
	--BGFGlab.Config.UI.Mode=text
fi

#run bgfglab for each cfg file
for f in ${CFGPATH}/*.cfg; do
    ALG=`basename $f .cfg`
    NALGDUMP=`ls ${DATADIR}/${ALG}.dump*.fgmask.pnm | wc -l`
    #dump
    if [ $NALGDUMP -ne $NIMG ]; then
	bgfglab --Ice.Config=$f \
	    --BGFGlab.Endpoints=default \
	    --BGFGlab.Config.ImageProvider.Source=1 \
	    --BGFGlab.Config.ImageProvider.LocalSourcePath=${DATADIR}/input%05d.pnm \
	    --BGFGlab.Config.BGAlgorithm.InitialImg=${IDEALBG} \
	    --BGFGlab.Config.Dump.File=${DATADIR}/${ALG}.dump%05d \
	    --BGFGlab.Config.Dump.Frames=${NIMG} \
	    --BGFGlab.Config.Dump.DumpIMG=0 \
	    --BGFGlab.Config.Dump.DumpBG=0 \
	    --BGFGlab.Config.Dump.DumpFG=1 \
	    --BGFGlab.Config.UI.Mode=text
    fi

    #mix ideal and calculated fg in one image. ideal->green channel cacl->red channel
    #concatenate img with fg cmp
    NALGOUT=`ls ${DATADIR}/${ALG}.out*.jpg | wc -l`
    if [ $NALGOUT -ne $NIMG ]; then
	for i in `seq $NIMG`; do
	    GREEN=`printf ${DATADIR}/ideal.dump%05d.fgmask.pnm $i`
	    RED=`printf ${DATADIR}/${ALG}.dump%05d.fgmask.pnm $i`
	    CMP=`printf ${DATADIR}/${ALG}.cmp%05d.pnm $i`
	    INPUT=`printf ${DATADIR}/input%05d.pnm $i`
	    OUTPUT=`printf ${DATADIR}/${ALG}.out%05d.jpg $i`
	    convert $RED $GREEN -background black -channel red,green -combine $CMP
	    montage -mode Concatenate $INPUT $CMP $OUTPUT
	done
    fi

    #generate flv sequences for each alg: generated on CWD
    if [ ! -e ${ALG}.out.flv ]; then
	ffmpeg -f image2 -r $FPS -i ${DATADIR}/${ALG}.out%05d.jpg ${ALG}.out.flv
    fi

    #compare idealfg with each calculated fg
    if [ ! -e ${DATADIR}/${ALG}.cmp ]; then
	cmpFGSequences.py $NIMG ${DATADIR}/${ALG}.dump%05d.fgmask.pnm ${DATADIR}/ideal.dump%05d.fgmask.pnm > ${DATADIR}/${ALG}.cmp
    fi
done

rocAnalysis.R ${DATADIR}/*.cmp
