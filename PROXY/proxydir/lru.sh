filelist=`ls -lrtu | awk '{ if(NF!= 2 && $9!="lru.sh" && $9!="proxy.cpp" && $9!="proxy" ) print $9}'`
lines=`echo "$filelist" | wc -l`
echo "$filelist"
echo "#lines = $lines"
if [ $lines -gt 5 ]
then 
filename=`echo $filelist |  awk '{if(NR==1) print $2}' `
echo "removing $filename"
rm $filename
fi
