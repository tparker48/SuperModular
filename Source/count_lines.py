import os

total_lines = 0

for root, dirs, files in os.walk('.'):
    for file in files:
        if file.endswith('.cpp') or file.endswith('.h'):
            with open(os.path.join(root, file), 'r') as f:
                lines = f.readlines()
                print(f"{file}: {len(lines)} lines")
                total_lines += len(lines)

print(f"Total lines of code: {total_lines}")