import os

total_lines = 0

for x,y,z in os.walk('.'):
    for file in z:
        if file.endswith('.cpp') or file.endswith('.h'):
            with open(os.path.join(x, file), 'r') as f:
                lines = f.readlines()
                print(f"{file}: {len(lines)} lines")
                total_lines += len(lines)

print(f"Total lines of code: {total_lines}")