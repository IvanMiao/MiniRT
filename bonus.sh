# Navigate to the bonus/src directory
cd /home/ymiao/Documents/projet/minirt-github/bonus/src

# Rename all .c files to add _bonus suffix
find . -name "*.c" -type f | while read file; do
    dir=$(dirname "$file")
    base=$(basename "$file" .c)
    mv "$file" "$dir/${base}_bonus.c"
done

# Rename all .h files to add _bonus suffix
find . -name "*.h" -type f | while read file; do
    dir=$(dirname "$file")
    base=$(basename "$file" .h)
    mv "$file" "$dir/${base}_bonus.h"
done
