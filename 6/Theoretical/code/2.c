// Transaction 1
SLock(n)
XLock(m)
read(n)
read(m)
if (n == 0) m++
write(m)
XUnlock(m)
SUnlock(n)


// Transaction 2
XLock(n)
SLock(m)
read(m)
read(n)
if (m == 0) n++
write(n)
SUnlock(m)
XUnlock(n)