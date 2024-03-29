from threading import Thread
import requests
import time

def request_site():
    return requests.get('http://www.example.org').text

start_time = time.time()
threads = []
for i in range(20):
    thread = Thread(target = request_site)
    thread.start()
    threads.append(thread)

for t in threads:
    t.join()

end_time = time.time()
print(end_time - start_time)
