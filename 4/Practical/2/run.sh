for i in {1..20}; do
	time python3 async_programming.py
done
echo "============"
for i in {1..20}; do
    time python3 thread.py
done
