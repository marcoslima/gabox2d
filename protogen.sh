mkdir -p genproto
rm -rf genproto/*

for module in $(find ./protos/* -type d); do
  protoc -I ./protos -I ${module} \
      --cpp_out=genproto/ \
      ${module}/*.proto
done
