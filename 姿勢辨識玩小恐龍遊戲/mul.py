import threading
import subprocess

# 啟動 Chrome Dino 遊戲
def run_dino_game():
    subprocess.run(["python", "dino.py"])

# 啟動姿勢檢測
def run_pose_detection():
    subprocess.run(["python", "run_webcam.py"])

if __name__ == "__main__":
    # 創建兩個
    thread1 = threading.Thread(target=run_dino_game)
    thread2 = threading.Thread(target=run_pose_detection)

    # 啟動
    thread1.start()
    thread2.start()

    # 等待兩個程式結束
    thread1.join()
    thread2.join()
