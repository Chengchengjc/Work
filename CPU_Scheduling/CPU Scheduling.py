import copy
import queue
import time

class Process:
    def __init__(self, process_id, cpu_burst, arrival_time, priority):
        self.process_id = process_id
        self.cpu_burst = cpu_burst
        self.rest_cpu_burst = cpu_burst
        self.arrival_time = arrival_time
        self.priority = priority
        self.start_time = None
        self.finish_time = None    

class ProcessScheduler:
    def __init__(self):
        self.processes = []
        self.task_id = None
        self.time_slice = None

    def read_processes_from_file(self, file_path):
        with open(file_path, 'r') as file:
            lines = file.readlines()
            # 處理task_id and time_slice
            self.task_id, self.time_slice = map(int, lines[0].strip().split())
            
            for line in lines[2:]:
                data = line.strip().split()
                if not data :
                    break
                
                # process_id = numtoletter(int(data[0])) if int(data[0]) > 9 else data[0]
                process_id = int(data[0])
                cpu_burst = int(data[1])
                arrival_time = int(data[2])
                priority = int(data[3])
                process = Process(process_id, cpu_burst, arrival_time, priority)
                self.processes.append(process)
                
    def sort_processes(self): # sort by arrival time
        self.processes.sort(key=lambda x: (x.arrival_time, x.process_id))
    def sort_order(self):
        self.processes.sort(key=lambda x: x.process_id)
    def sort_arrival_burst(self): # sort by arrival time and burst time
        self.processes.sort(key=lambda x: (x.arrival_time, x.cpu_burst, x.process_id))        
    def sort_short_first(self):
        self.processes.sort(key=lambda x: (x.cpu_burst, x.arrival_time, x.process_id))
    def sort_rest_short_first(self):
        self.processes.sort(key=lambda x: (x.rest_cpu_burst, x.arrival_time, x.process_id))
                
    def sort_arrival_priority(self):
        self.processes.sort(key=lambda x: (x.arrival_time, x.priority, x.process_id))
                        
        
def numtoletter(num):
    list = ['A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O',
            'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z']
    return list[num-10]
def FCFS(scheduler):
    scheduler.sort_processes()
    gantt_chart = ""
    current_Time = scheduler.processes[0].arrival_time
    for i in range(0, current_Time):
        gantt_chart += str('-')
        
    for process in scheduler.processes:
        if current_Time >= process.arrival_time:
            process.start_time = current_Time
            process.finish_time = current_Time + process.cpu_burst
            current_Time += process.cpu_burst
        else: 
            empty_time = process.arrival_time - current_Time # 此刻沒有process
            for _ in range(empty_time):
                gantt_chart += str("-")
            current_Time = process.arrival_time
            process.start_time = process.arrival_time
            process.finish_time = process.arrival_time + process.cpu_burst
            current_Time += process.cpu_burst
            
        for _ in range(process.cpu_burst):
            gantt_chart += str(numtoletter(process.process_id)) if process.process_id > 9 else str(process.process_id)
            
    return gantt_chart
    
def getDoingTime(process, time_slice):
    if process.rest_cpu_burst >= time_slice:
        time = time_slice
    else:
        time = process.rest_cpu_burst  
    return time    

def RR(scheduler):
    scheduler.sort_processes()

    ready_queue = queue.Queue()
    running_task = [] # store(running time, process)
    temp_for_task = []
    done_process = []
    gantt_chart = ""
    current_time = scheduler.processes[0].arrival_time
    for _ in range(current_time):
        gantt_chart += str('-')
    
    running_task.append((getDoingTime(scheduler.processes[0], scheduler.time_slice), scheduler.processes[0]))
    while len(done_process) < len(scheduler.processes):
        for process in scheduler.processes[1:]:
            if process.arrival_time == current_time: # set ready queue
                ready_queue.put(process)
                
        if temp_for_task : # 新來的process放在重新進入ready queue的process的前面
            ready_queue.put(temp_for_task[0])
            temp_for_task.pop(0)
            
        if not running_task and not ready_queue.empty():
            next_task = ready_queue.get()
            running_task.append((getDoingTime(next_task, scheduler.time_slice), next_task))
            
        if running_task:
            if running_task[0][1].start_time == None:
                running_task[0][1].start_time = current_time  # 更新當前任務的開始時間
                
            running_task[0] = (running_task[0][0] - 1, running_task[0][1])  # 更新當前任務剩餘值行時間
            running_task[0][1].rest_cpu_burst -= 1
            
            # 紀錄當前任務的ID
            gantt_chart += str(numtoletter(running_task[0][1].process_id)) if running_task[0][1].process_id > 9 else str(running_task[0][1].process_id) 
            if running_task[0][0] == 0:  # time slice用完
                if running_task[0][1].rest_cpu_burst != 0:
                    temp_for_task.append(running_task[0][1])
                    # ready_queue.put(running_task[0][1])
                else: # rest_cpu_burst == 0
                    done_process.append(running_task[0][1])
                    running_task[0][1].finish_time = current_time + 1
    
                running_task.pop(0)  # running task 任務完成 去ready queue找下一個
                if not ready_queue.empty():
                    next_task = ready_queue.get()
                    running_task.append((getDoingTime(next_task, scheduler.time_slice), next_task))
        else:
            gantt_chart += str('-')  # 當前沒有任務在執行

        current_time += 1  # 更新當前時間
        
    return gantt_chart
        
def SJF(scheduler):
    # sort
    scheduler.sort_arrival_burst() # 先找第一個抵達且burst time最小的
    first_element = scheduler.processes[0]
    scheduler.processes.pop(0)
    scheduler.sort_short_first() # 之後的照burst time and arrival time 排序
    scheduler.processes.insert(0, first_element)
    
    done_process =[]
    done = False
    gantt_chart = ""
    current_time = scheduler.processes[0].arrival_time
    for _ in range(current_time):
        gantt_chart += str('-')

    while True:
        done = False
        for process in scheduler.processes:
            if process not in done_process and current_time >= process.arrival_time:
                done_process.append(process)
                
                for _ in range(process.cpu_burst): # draw gantt chart
                    gantt_chart += str(numtoletter(process.process_id)) if process.process_id > 9 else str(process.process_id)
        
                process.start_time = current_time
                process.finish_time = current_time + process.cpu_burst
                current_time += process.cpu_burst # update current time
                done = True
                break
            
        if not done and len(done_process) < len(scheduler.processes): # 此刻沒有process要做,且還沒做完
            current_time += 1
            gantt_chart += str('-')  
            scheduler.sort_arrival_burst() 
        elif done:
            scheduler.sort_short_first()       
        elif not done: # 結束
            break
        
    return gantt_chart

def SRTF(scheduler):
    # sort
    scheduler.sort_arrival_burst() # 先找第一個抵達且burst time最小的
    first_element = scheduler.processes[0]
    scheduler.processes.pop(0)
    scheduler.sort_short_first() # 之後的照burst time and arrival time 排序
    scheduler.processes.insert(0, first_element)
        
    done_process = []
    gantt_chart = ""
    current_time = scheduler.processes[0].arrival_time
    for _ in range(current_time):
        gantt_chart += str('-')

    while True:
        done = False
        for process in scheduler.processes:
            if process not in done_process and current_time >= process.arrival_time:
                gantt_chart += str(numtoletter(process.process_id)) if process.process_id > 9 else str(process.process_id)
                process.rest_cpu_burst -= 1 # update cpu burst time
                
                if process.start_time == None:
                    process.start_time = current_time
                    
                if process.rest_cpu_burst == 0:
                    done_process.append(process)
                    process.finish_time = current_time + 1
                    
                current_time += 1 #update current time
                done = True
                break
        
        if done:
            scheduler.sort_rest_short_first() # 每次做完依照剩餘cpu burst time 排序
        elif not done and len(done_process) < len(scheduler.processes): # 此刻沒有process要做
            current_time += 1
            gantt_chart += str('-')
            scheduler.sort_arrival_burst() # 先找第一個抵達且burst time最小的
        else: # 結束
            break
        
    return gantt_chart    
    
def HRRN(scheduler):
    # sort
    scheduler.sort_processes()

    done_process = []
    ratio_list = []
    gantt_chart = ""
    current_time = scheduler.processes[0].arrival_time
    for _ in range(current_time):
        gantt_chart += str('-')
        
    scheduler.processes[0].start_time = current_time
    scheduler.processes[0].finish_time = current_time + scheduler.processes[0].cpu_burst
    current_time += scheduler.processes[0].cpu_burst # update current time
    done_process.append(scheduler.processes[0])
    for _ in range(scheduler.processes[0].cpu_burst):
        gantt_chart += str(numtoletter(scheduler.processes[0].process_id)) if scheduler.processes[0].process_id > 9 else str(scheduler.processes[0].process_id)    
    
    while len(done_process) < len(scheduler.processes):
        for process in scheduler.processes:
            if process not in done_process and process.arrival_time <= current_time:
                ratio = ((current_time - process.arrival_time) + process.cpu_burst ) / process.cpu_burst
                ratio_list.append((ratio, process))
        
        if ratio_list:
            next_process = ratio_list[0][1]    
            max_ratio = ratio_list[0][0]     
            for i in range(1, len(ratio_list)): # find max ratio
                if ratio_list[i][0] > max_ratio:
                    next_process = ratio_list[i][1]
                    max_ratio = ratio_list[i][0]
        else: # 若此刻沒有process在做
            for process in scheduler.processes:
                if process not in done_process:
                    next_process = process # 取還沒做過裡抵達時間最小的
                    for i in range(next_process.arrival_time - current_time):
                        gantt_chart += str('-')
                    current_time = next_process.arrival_time
                    break
                
        next_process.start_time = current_time
        next_process.finish_time = current_time + next_process.cpu_burst
        current_time += next_process.cpu_burst # update current time
        for i in range(next_process.cpu_burst):
            gantt_chart += str(numtoletter(next_process.process_id)) if next_process.process_id > 9 else str(next_process.process_id)    
        done_process.append(next_process)
        ratio_list = [] # clear ratio list for next iteration
        
    return gantt_chart
def add_to_ready_queue(process, ready_queue):
    done = False
    for i in range(len(ready_queue)):
        if process.priority < ready_queue[i].priority:
            ready_queue.insert(i, process)
            done = True
            break
    if not done:
        ready_queue.append(process)   
         
def PPRR(scheduler):
    # sort
    scheduler.sort_arrival_priority() 

    ready_queue = []
    running_task = [] # store(running time, process) 存剩餘執行時間
    done_process = []
    gantt_chart = ""
    current_time = scheduler.processes[0].arrival_time
    for _ in range(current_time):
        gantt_chart += str('-')
    
    running_task.append((getDoingTime(scheduler.processes[0], scheduler.time_slice), scheduler.processes[0]))
    while len(done_process) < len(scheduler.processes):

        for process in scheduler.processes[1:]:
            if process.arrival_time == current_time: # set ready queue
                add_to_ready_queue(process, ready_queue)
                
        if not running_task and ready_queue:
            for process in ready_queue:
                if current_time >= process.arrival_time:
                    next_task = process
                    ready_queue.remove(process)
                    running_task.append((getDoingTime(next_task, scheduler.time_slice), next_task))
                    break
        
        if running_task:
            if ready_queue: # 篡位
                if running_task[0][1].priority > ready_queue[0].priority:
                    add_to_ready_queue(running_task[0][1], ready_queue)
                    running_task[0] = ((getDoingTime(ready_queue[0], scheduler.time_slice), ready_queue[0]))
                    ready_queue.pop(0)
                
            if running_task[0][1].start_time == None:
                running_task[0][1].start_time = current_time  # 更新當前任務的開始時間
                
            running_task[0] = (running_task[0][0] - 1, running_task[0][1])  # 更新當前任務剩餘值行時間
            running_task[0][1].rest_cpu_burst -= 1
            
            # 紀錄當前任務的ID
            gantt_chart += str(numtoletter(running_task[0][1].process_id)) if running_task[0][1].process_id > 9 else str(running_task[0][1].process_id) 
            if running_task[0][0] == 0:  # time slice用完
                if running_task[0][1].rest_cpu_burst != 0:
                    add_to_ready_queue(running_task[0][1], ready_queue)

                else: # rest_cpu_burst == 0
                    done_process.append(running_task[0][1])
                    running_task[0][1].finish_time = current_time + 1
    
                running_task.pop(0)  # running task 任務完成 去ready queue找下一個
                      
        else:
            gantt_chart += str('-')  # 當前沒有任務在執行

        current_time += 1  # 更新當前時間
        
    return gantt_chart            
    
def writetoFIle(scheduler, method, filename, gantt_chart):
    with open(f"out_{filename}.txt", 'w') as file:
        if method == 'PPRR':
           file.write("Priority RR\n") 
        else :
            file.write(f"{method}\n")
        file.write('=={0:>12}==\n'.format(method))
        file.write(f"{gantt_chart}\n")
        file.write("=" * (len(gantt_chart)))
        file.write("\n\n")
        
        scheduler.sort_order() # 照id排序
        file.write("Waiting Time\n")
        file.write(f"ID	{method}\n")
        file.write("===========================================================\n")
        for process in scheduler.processes:
            waiting_time = process.finish_time - process.arrival_time - process.cpu_burst
            file.write(f"{process.process_id}\t{waiting_time}\n")
        file.write("===========================================================\n\n")
        
        file.write("Turnaround Time\n")
        file.write(f"ID	{method}\n")
        file.write("===========================================================\n")
        for process in scheduler.processes:
            Turnaround_time = process.finish_time - process.arrival_time
            file.write(f"{process.process_id}\t{Turnaround_time}\n")
        file.write("===========================================================\n\n")        
        
def writeAlltoFile(gantt_chart, scheduler_list, filename_without_extension):
    method = ["FCFS", "RR", "SJF", "SRTF", "HRRN", "PPRR"]
    id_num = len(scheduler_list[0].processes)
    with open(f"out_{filename_without_extension}.txt", 'w') as file:
        file.write("All\n")
        for i in range(6): # write gantt chart
            file.write('=={0:>12}==\n'.format(method[i]))
            file.write(f"{gantt_chart[i]}\n")
            
        file.write(f"=" * 59 )
        file.write("\n\n")
        
        for i in range(len(scheduler_list)):
            scheduler_list[i].sort_order() # 照id排序
            
        file.write("Waiting Time\n")
        file.write("ID\tFCFS\tRR\tSJF\tSRTF\tHRRN\tPPRR\n")
        file.write("=" * 59)
        file.write("\n")
        for i in range(id_num):
            file.write(f"{scheduler_list[0].processes[i].process_id}\t")
            for process in scheduler_list:
                waiting_time = process.processes[i].finish_time - process.processes[i].arrival_time - process.processes[i].cpu_burst
                if process != scheduler_list[-1]:
                    file.write(f"{waiting_time}\t")   
                else :
                    file.write(f"{waiting_time}")   
            file.write("\n")
                     
        file.write("=" * 59)
        file.write("\n\n")
        
        file.write("Turnaround Time\n")
        file.write("ID\tFCFS\tRR\tSJF\tSRTF\tHRRN\tPPRR\n")
        file.write("=" * 59)
        file.write("\n")

        for i in range(id_num):
            file.write(f"{scheduler_list[0].processes[i].process_id}\t")
            for process in scheduler_list:
                Turnaround_time = process.processes[i].finish_time - process.processes[i].arrival_time
                if process != scheduler_list[-1]:
                    file.write(f"{Turnaround_time}\t")  
                else :
                    file.write(f"{Turnaround_time}") 
                    
            file.write("\n")
        
        file.write("=" * 59)
        file.write("\n\n")        
                
if __name__ == "__main__":
    filename = input("Please enter File Name (eg. input1.txt): ")
    filename_without_extension = filename.split('.')[0] 

    scheduler = ProcessScheduler()
    scheduler.read_processes_from_file(filename)

    if scheduler.task_id == 1: # FCFS
        gantt_chart = FCFS(scheduler)
        writetoFIle(scheduler, "FCFS", filename_without_extension, gantt_chart)

    elif scheduler.task_id == 2: # RR
        gantt_chart = RR(scheduler)
        writetoFIle(scheduler, "RR", filename_without_extension, gantt_chart)
                
    elif scheduler.task_id == 3: # SJF
        gantt_chart = SJF(scheduler)
        writetoFIle(scheduler, "SJF", filename_without_extension, gantt_chart)
        
    elif scheduler.task_id == 4: # SRTF
        gantt_chart = SRTF(scheduler)
        writetoFIle(scheduler, "SRTF", filename_without_extension, gantt_chart)                
        
    elif scheduler.task_id == 5: # HRRN
        gantt_chart = HRRN(scheduler)
        writetoFIle(scheduler, "HRRN", filename_without_extension, gantt_chart)         
           
    elif scheduler.task_id == 6: # PPRR
        gantt_chart = PPRR(scheduler)
        writetoFIle(scheduler, "PPRR", filename_without_extension, gantt_chart)   
        
    elif scheduler.task_id == 7:  
        scheduler_list =[]
        gantt_chart = []
        for _ in range(6):
            scheduler_copy = copy.deepcopy(scheduler)
            scheduler_list.append(scheduler_copy)
            
        gantt_chart.append(FCFS(scheduler_list[0]))
        gantt_chart.append(RR(scheduler_list[1]))
        gantt_chart.append(SJF(scheduler_list[2]))
        gantt_chart.append(SRTF(scheduler_list[3]))
        gantt_chart.append(HRRN(scheduler_list[4]))
        gantt_chart.append(PPRR(scheduler_list[5]))
        
        writeAlltoFile(gantt_chart, scheduler_list, filename_without_extension)
            
        
            
                
    



