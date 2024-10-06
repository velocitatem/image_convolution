for size in 1 2 4 8 16 32
do
  echo "Running with $size threads:"
  export OMP_NUM_THREADS=$size
  ./cmake-build-debug/ImageConvolution
done
