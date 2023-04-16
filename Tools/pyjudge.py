import time
import logging
import argparse
import tempfile
import subprocess
from pathlib import Path

# Ansi Colors, https://en.wikipedia.org/wiki/ANSI_escape_code#Colors
class Ansi:
    color_green = "\x1b[32m"
    color_blue = "\x1b[34m"
    color_magenta = "\x1b[35m"
    color_grey = "\x1b[38;20m"
    color_yellow = "\x1b[33;20m"
    color_red = "\x1b[31;20m"
    color_bold_red = "\x1b[31;1m"
    color_reset = "\x1b[0m"

    def green(self,s): return self.color_green+s+self.color_reset
    def blue(self,s): return self.color_blue+s+self.color_reset
    def magenta(self,s): return self.color_magenta+s+self.color_reset
    def grey(self,s): return self.color_grey+s+self.color_reset
    def yellow(self,s): return self.color_yellow+s+self.color_reset
    def red(self,s): return self.color_red+s+self.color_reset
    def bold_red(self,s): return self.color_bold_red+s+self.color_reset
ansi=Ansi()

# 自定义log formatter，参考自： https://stackoverflow.com/questions/384076/how-can-i-color-python-logging-output
class CustomFormatter(logging.Formatter):

    def get_fmt(color):
        return "["+color("%(levelname)s")+"] %(message)s"

    FORMATS = {
        # 对WARN做了长度对齐
        logging.DEBUG: get_fmt(ansi.grey),
        logging.INFO: get_fmt(ansi.blue),
        logging.WARNING: "["+ansi.yellow("WARN")+"] %(message)s",
        logging.ERROR: get_fmt(ansi.red),
        logging.CRITICAL: get_fmt(ansi.bold_red)
    }

    def format(self, record):
        log_fmt = self.FORMATS.get(record.levelno)
        formatter = logging.Formatter(log_fmt)
        return formatter.format(record)

log=logging.getLogger('pyjudge')

class Judger:

    # exe=编译完的二进制程序路径名，out=临时输出文件路径名，dir=测试数据目录名，tle=时限，std=是否当作标程处理
    def run(self,exe,out,dir,tle,std):
        accepted,total,slowest=0,0,0

        def _run(outp):
            try:
                # 较为粗略的时间计算
                start=time.time()
                process=subprocess.run(args=exe,stdin=inp.open(),stdout=outp.open('w'),timeout=tle,check=True)
                end=time.time()
            except subprocess.CalledProcessError as err:
                # log.error(err)
                log.warning(inp.name+ansi.magenta(" Runtime Error"))
                return False,0
            except subprocess.TimeoutExpired as err:
                log.warning(inp.name+ansi.blue(" Time Limit Exceeded"))
                return False,tle
            return True,end-start

        # 获取目录树下所有的.in文件
        for inp in Path(dir).rglob('*.in'):
            # 计算对应的.out文件路径
            outp=inp.parent.joinpath(inp.name[:-3]+'.out')
            # 如果是标程，那么生成答案输出到.out文件
            if(std):
                if(not outp.exists()): outp.touch()
                good,duration=_run(outp)
                if(good):
                    log.info(inp.name+ansi.green(' Finished')+', Executed in '+ansi.green(str(int(duration*1000)))+' ms')
                slowest=max(slowest,duration)

            # 否则，将标准输出与.out文件进行对比
            elif(outp.exists()):
                total+=1

                # 检查数据文件编码（但是不检查测试代码的输出）
                def check_decode(file_path):
                    try:
                        with file_path.open() as f: f.readline()
                    except UnicodeDecodeError as err:
                        log.error(file_path.name+ansi.bold_red(" unknown file encoding"))
                        return False
                    return True

                if(not check_decode(inp)): continue
                if(not check_decode(outp)): continue

                ac,line_cnt=1,0
                good,duration=_run(Path(out))
                slowest=max(slowest,duration)
                if(not good): continue

                with outp.open() as ans, Path(out).open() as res:
                    while True:
                        line_cnt+=1
                        line_res=res.readline()
                        line_ans=ans.readline()
                        # 将每行按空格拆分成list进行比较
                        if line_res.split() != line_ans.split(): ac=0
                        if not line_res or not line_ans or not ac: break

                if(ac):
                    log.info(inp.name+ansi.green(' Accepted')+', Executed in '+ansi.green(str(int(duration*1000)))+' ms')
                else:
                    log.warning(inp.name+ansi.red(' Wrong Answer')+' on line '+ansi.red(str(line_cnt))+', Executed in '+ansi.red(str(int(duration*1000)))+' ms')
                accepted+=ac

        return accepted,total,slowest

    def __init__(self) -> None:
        pass


def main():
    parser=argparse.ArgumentParser(
        prog="pyjudge",
        description="A simple competitive programming judger wrote in python"
    )
    parser.add_argument('filename',help='the C++ Code to judge')
    parser.add_argument('-d','--dir',default='data',help='dirctory of testcases, default is "data"')
    parser.add_argument('-t','--tle',default=1,help='time limit, default is 1 second')
    parser.add_argument('-s','--save',action='store_true',help='save output to .out file')
    parser.add_argument('-v','--version',action='version',version='%(prog)s 1.0.2 by HEltim7')
    args=parser.parse_args()

    ch = logging.StreamHandler()
    ch.setLevel(logging.DEBUG)
    ch.setFormatter(CustomFormatter())
    log.setLevel(logging.INFO)
    log.addHandler(ch)

    log.info('Compiling...')
    work_dir=tempfile.TemporaryDirectory(prefix='_pyjudge_')
    exe_file=work_dir.name+'/test'
    out_file=work_dir.name+'/test.out'

    try:
        subprocess.run(['g++','-o',exe_file,'-std=c++20','-O2','-DONLINE_JUDGE',args.filename],check=True)
    except subprocess.CalledProcessError as err:
        log.error(ansi.bold_red("Compile Error"))
        return

    log.info('Running...')
    judger=Judger()
    accepted,total,slowest=judger.run(exe_file,out_file,args.dir,float(args.tle),args.save)
    work_dir.cleanup()

    log.info('')
    if(not args.save): log.info('Accepted '+str(accepted)+' / '+str(total))
    log.info('Slowest: '+ansi.green(str(int(slowest*1000)))+' ms ')

if __name__ == "__main__":
    start=time.time()
    main()
    end=time.time()
    log.info("Finished in %.3f s\n"%(end-start))