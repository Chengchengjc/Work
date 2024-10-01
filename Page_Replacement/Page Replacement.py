class Page_info:
    def __init__(self, str, page, fault):
        self.str = str
        self.page = page
        self.fault = fault
    
def read_file(fileName):
    refString = []
    with open(fileName, 'r') as file:
        lines = file.readlines()
        method, frame_size = map(int, lines[0].strip().split())
        refString = [ char for char in lines[1].strip()]

    return method, frame_size, refString   

def write_file(filename_without_ext, result, page_fault, page_replaces, frame_size, method ):
    with open(f"out_{filename_without_ext}.txt", 'w') as file:
        file.write(f"{'-'*14}{method}{'-'*23}\n")        
        for i in range(len(result)):
            if result[i].fault == '':
                file.write(f"{result[i].str}\t{result[i].page}\n")
            else:
                file.write(f"{result[i].str}\t{result[i].page}\t{result[i].fault}\n")
        file.write(f"Page Fault = {page_fault}  Page Replaces = {page_replaces}  Page Frames = {frame_size}\n")       
def FIFO(refString, frame_size):
    page_frame = []
    frame_list = []
    page_fault = 0
    page_replaces = 0
    
    for i in range(len(refString)):
        if refString[i] not in page_frame:
            page_fault += 1
            if len(page_frame) < frame_size: 
                # page fult and not page replacement
                page_frame.append(refString[i])
                p = Page_info(refString[i], ''.join(page_frame[::-1]), 'F')
            else:
                # page fult and page replacement
                page_replaces += 1
                page_frame.pop(0)
                page_frame.append(refString[i])
                p = Page_info(refString[i], ''.join(page_frame[::-1]), 'F')
        else:
            # refString[i] in frame
            p = Page_info(refString[i], ''.join(page_frame[::-1]), '')
            
        frame_list.append(p)
    return frame_list, page_fault, page_replaces       
            
def LRU(refString, frame_size):
    page_frame = []
    frame_list = []
    page_fault = 0
    page_replaces = 0
    
    for i in range(len(refString)):
        if refString[i] not in page_frame:
            page_fault += 1
            if len(page_frame) < frame_size: 
                # page fult and not page replacement
                page_frame.append(refString[i])
                p = Page_info(refString[i], ''.join(page_frame[::-1]), 'F')
            else:
                # page fult and page replacement
                page_replaces += 1
                page_frame.pop(0)
                page_frame.append(refString[i])
                p = Page_info(refString[i], ''.join(page_frame[::-1]), 'F')
        else:
            # refString[i] in frame
            index = page_frame.index(refString[i])
            # LRU put to the beginning of the page
            page_frame.pop(index) 
            page_frame.append(refString[i]) 
            p = Page_info(refString[i], ''.join(page_frame[::-1]), '')

        frame_list.append(p)
    return frame_list, page_fault, page_replaces
def LFU(refString, frame_size):
    page_frame = []
    frame_list = []
    freq_list = {}
    page_fault = 0
    page_replaces = 0
    
    # initialize counter
    for i in range(len(refString)):
        freq_list[refString[i]] = 0
        
    for i in range(len(refString)):
        if refString[i] not in page_frame:
            page_fault += 1
            freq_list[refString[i]] = 1
            if len(page_frame) < frame_size: 
                # page fult and not page replacement
                page_frame.append(refString[i])
                p = Page_info(refString[i], ''.join(page_frame[::-1]), 'F')
            else:
                # page fult and page replacement
                page_replaces += 1
                idx = find_smallest_counter(freq_list, page_frame)
                
                freq_list[page_frame[idx]] = 0 # reset
                page_frame.pop(idx)
                
                page_frame.append(refString[i]) # put new to page
                p = Page_info(refString[i], ''.join(page_frame[::-1]), 'F')
        else:
            # refString[i] in frame
            freq_list[refString[i]] += 1 
            p = Page_info(refString[i], ''.join(page_frame[::-1]), '')
            
        frame_list.append(p)
    return frame_list, page_fault, page_replaces    

def MFU(refString, frame_size):
    page_frame = []
    frame_list = []
    freq_list = {}
    page_fault = 0
    page_replaces = 0
    
    # initialize counter
    for i in range(len(refString)):
        freq_list[refString[i]] = 0
        
    for i in range(len(refString)):
        if refString[i] not in page_frame:
            page_fault += 1
            freq_list[refString[i]] = 1
            if len(page_frame) < frame_size: 
                # page fult and not page replacement
                page_frame.append(refString[i])
                p = Page_info(refString[i], ''.join(page_frame[::-1]), 'F')
            else:
                # page fult and page replacement
                page_replaces += 1
                idx = find_largest_counter(freq_list, page_frame)
                
                freq_list[page_frame[idx]] = 0 # reset
                page_frame.pop(idx)
                
                page_frame.append(refString[i]) # put new to page
                p = Page_info(refString[i], ''.join(page_frame[::-1]), 'F')
        else:
            # refString[i] in frame
            freq_list[refString[i]] += 1 
            p = Page_info(refString[i], ''.join(page_frame[::-1]), '')
            
        frame_list.append(p)
    return frame_list, page_fault, page_replaces 

def find_largest_counter(freq_list, page_frame):
    index = 0
    max = freq_list[page_frame[0]]
    for i in range(1, len(page_frame)): # find minimum counter
        if freq_list[page_frame[i]] > max:
            index = i
            max = freq_list[page_frame[i]]
    return index 
      
def LFU_LRU(refString, frame_size):
    page_frame = []
    frame_list = []
    freq_list = {}
    page_fault = 0
    page_replaces = 0
    
    # initialize counter
    for i in range(len(refString)):
        freq_list[refString[i]] = 0
        
    for i in range(len(refString)):
        if refString[i] not in page_frame:
            page_fault += 1
            freq_list[refString[i]] = 1
            if len(page_frame) < frame_size: 
                # page fult and not page replacement
                page_frame.append(refString[i])
                p = Page_info(refString[i], ''.join(page_frame[::-1]), 'F')
            else:
                # page fult and page replacement
                page_replaces += 1
                idx = find_smallest_counter(freq_list, page_frame)
                
                freq_list[page_frame[idx]] = 0 # reset
                page_frame.pop(idx)
                
                page_frame.append(refString[i]) # put new to page
                p = Page_info(refString[i], ''.join(page_frame[::-1]), 'F')
        else:
            # refString[i] in frame
            freq_list[refString[i]] += 1 

            index = page_frame.index(refString[i])
            # LRU put to the beginning of the page
            page_frame.pop(index) 
            page_frame.append(refString[i]) 
            p = Page_info(refString[i], ''.join(page_frame[::-1]), '')
            
        frame_list.append(p)
    return frame_list, page_fault, page_replaces  
      
def find_smallest_counter(freq_list, page_frame):
    index = 0
    min = freq_list[page_frame[0]]
    for i in range(1, len(page_frame)): # find minimum counter
        if freq_list[page_frame[i]] < min:
            index = i
            min = freq_list[page_frame[i]]
    return index        
def write_all_to_file(filename_without_ext, all_results, all_fault, all_replaces, frame_size, method ):
    with open(f"out_{filename_without_ext}.txt", 'w') as file:
        for j in range(len(all_results)):
            file.write(f"{'-'*14}{method[j]}{'-'*23}\n")        
            for i in range(len(all_results[j])):
                if all_results[j][i].fault == '':
                    file.write(f"{all_results[j][i].str}\t{all_results[j][i].page}\n")
                else:
                    file.write(f"{all_results[j][i].str}\t{all_results[j][i].page}\t{all_results[j][i].fault}\n")
            file.write(f"Page Fault = {all_fault[j]}  Page Replaces = {all_replaces[j]}  Page Frames = {frame_size}\n")
            
            if j < len(all_results)-1:
                file.write("\n") 
def build_results(result, page_fault, page_replaces, all_results, all_replaces, all_fault):
    all_results.append(result)
    all_replaces.append(page_replaces)
    all_fault.append(page_fault)         
    return all_results, all_replaces, all_fault       
                                      
if __name__ == "__main__":        
    filename = input("Please enter File Name (eg. input1.txt): ")
    filename_without_ext = filename.split('.')[0]
    method, frame_size, refString = read_file(filename)
    Method_name = ["FIFO", "LRU", "Least Frequently Used Page Replacement", "Most Frequently Used Page Replacement ", "Least Frequently Used LRU Page Replacement"]
    result = []
    all_results = []
    all_replaces = []
    all_fault = []
    if method == 1:
        result, page_fault, page_replaces = FIFO(refString, frame_size)
        write_file(filename_without_ext, result, page_fault, page_replaces, frame_size, Method_name[0])
    elif method == 2:
        result, page_fault, page_replaces = LRU(refString, frame_size)
        write_file(filename_without_ext, result, page_fault, page_replaces, frame_size, Method_name[1])
    elif method == 3:
        result, page_fault, page_replaces = LFU(refString, frame_size)
        write_file(filename_without_ext, result, page_fault, page_replaces, frame_size, Method_name[2])   
    elif method == 4:
        result, page_fault, page_replaces = MFU(refString, frame_size)
        write_file(filename_without_ext, result, page_fault, page_replaces, frame_size, Method_name[3])
    elif method == 5:
        result, page_fault, page_replaces = LFU_LRU(refString, frame_size)
        write_file(filename_without_ext, result, page_fault, page_replaces, frame_size, Method_name[4])
    elif method == 6:
        result, page_fault, page_replaces = FIFO(refString, frame_size)
        all_results, all_replaces, all_fault = build_results(result, page_fault, page_replaces, all_results, all_replaces, all_fault)
                                
        result, page_fault, page_replaces = LRU(refString, frame_size)
        all_results, all_replaces, all_fault = build_results(result, page_fault, page_replaces, all_results, all_replaces, all_fault)
              
        result, page_fault, page_replaces = LFU(refString, frame_size)
        all_results, all_replaces, all_fault = build_results(result, page_fault, page_replaces, all_results, all_replaces, all_fault)
              
        result, page_fault, page_replaces = MFU(refString, frame_size)
        all_results, all_replaces, all_fault = build_results(result, page_fault, page_replaces, all_results, all_replaces, all_fault)
        
        result, page_fault, page_replaces = LFU_LRU(refString, frame_size)
        all_results, all_replaces, all_fault = build_results(result, page_fault, page_replaces, all_results, all_replaces, all_fault)
                
        write_all_to_file(filename_without_ext, all_results, all_fault, all_replaces, frame_size, Method_name)
        
  