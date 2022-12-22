// Transaction 1
XLock(m)
SLock(n)
if (n == 0) m++
SUnlock(n)
XUnlock(m)

// Transaction 2
XLock(n)
SLock(m)
if (m == 0) n++
SUnlock(m)
XUnlock(n)
