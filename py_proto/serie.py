import os, time 

def old_scannER(birth = None):
    pnamId = {}
    if birth is None:
        for proc in os.listdir('/proc/'):
            if not proc.isdigit():
                continue
    
            pid = int(proc)
            try:
                with open(f'/proc/{pid}/stat') as f:
                    x = f.read()
            except FileNotFoundError:
                continue
    
            y = tokenx(x)
            pnamId[pid] = y 
    else:
        for born in birth:
            try:
                with open(f'/proc/{born}/stat') as f:
                    x = f.read()
            except FileNotFoundError:
                continue

            y = tokenx(x)
            pnamId[born] = y
    
    return pnamId

processes = {}

def scanER_pid():
    pid = set()
    for pro in os.listdir('/proc/'):
        if pro.isdigit():
            pid.add(int(pro))
    return pid

def get_stat(pid):
    pnamId = {}
    for i in pid:
        try:
            with open(f'/proc/{i}/stat') as f:
                x = f.read()
        except FileNotFoundError:
            continue
        y = tokenx(x)
        #print(y) # D BUG
        pnamId[i] = y
    
    processes.update(pnamId)
    return pnamId

def tokenx(x):
    a = 0
    for i,val in enumerate(x):
        if val == '(':  
            a = i+1
        if val == ')':
            b = i

    name = x[a:b]
    x = x[b+2:]
    y = list()
    y.append(name)
    y.extend(x.split()[:2])
    return y 

def compare(prev, now):
    death = prev - now
    birth = now - prev
    get_stat(birth)

    for i in death:
        print(f"Death \n PID = {i} name = {processes.get(i, 'unknown')}")
        processes.pop(i, None)   
    for j in birth:
        print(f"Borth \n PID = {j} name = {processes.get(j, 'unknown')}") 
    return birth

def tre_buil():
    childr = {}
    for pid, info in processes.items():
        name, state, ppid = info
        parent = int(ppid)

        if parent not in childr:
            childr[parent] = []
        childr[parent].append(pid)
    return childr

def tre_sho(childr, pid = 1, lev = 0):
    if pid not in processes:
        return
    name = processes[pid][0]
    print(" "*lev + f"{pid} {name}")
    
    for child in childr.get(pid, []):
        tre_sho(childr, child, lev + 4)
    
def check_fd(pids):
    x = {}
    for pid in pids:
        try:
            x[pid] = os.listdir(f'/proc/{pid}/fd')
            x[pid] = get_link(x[pid],pid)
        except FileNotFoundError:
            continue
        except PermissionError:
            continue
    return x

def get_link(fds,pid):
    x = {}
    for fd in fds:
        path = f"/proc/{pid}/fd/{fd}"
        try:
            x[fd] = os.readlink(path)
        except FileNotFoundError:
            continue
        except PermissionError:
            continue
    return x

prev = scanER_pid()
compare(set(),prev)

while True:
    print(f"{len(prev)} processes")
    time.sleep(1)
    now = scanER_pid()
    print(f"{len(now)} processes")
    births = compare(prev,now)
    prev = now
    tree = tre_buil()
    tre_sho(tree,1)
    #print("_------fds-------_")
    #print(check_fd(births))
    
