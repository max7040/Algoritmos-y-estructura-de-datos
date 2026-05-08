from collections import deque

cola = deque()

for x in [3,6,9,12]:
    cola.append(x)

print("Cola inicial:", list(cola))

cola.popleft()

print("Frente actual:", cola[0])

print("Cola final:", list(cola))