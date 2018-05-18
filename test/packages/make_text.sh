files=$(ls packages)
for f in $files; do
	name="${f%.*}"
	echo "$name"
	mkdir $name
	cp camviz/Dockerfile camviz/search_file.sh packages/$f $name
	cd $name
	sed -i.back s:files:$name:g Dockerfile
	rm Dockerfile.back
	cd ..
done
