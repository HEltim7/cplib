import os
import time
import shutil
import logging
import argparse
import tempfile
import itertools 
import traceback
import subprocess
from pathlib import Path

_DEBUG=False

# [Ansi Colors](https://en.wikipedia.org/wiki/ANSI_escape_code#Colors)
class Ansi:
    color_green = '\x1b[32m'
    color_blue = '\x1b[34m'
    color_magenta = '\x1b[35m'
    color_grey = '\x1b[38m'
    color_yellow = '\x1b[33m'
    color_red = '\x1b[31m'
    color_bold_red = '\x1b[31m'
    color_reset = '\x1b[0m'
    color_blue_underline = '\x1b[34;4m'

    def green(self,s): return self.color_green+s+self.color_reset
    def blue(self,s): return self.color_blue+s+self.color_reset
    def magenta(self,s): return self.color_magenta+s+self.color_reset
    def grey(self,s): return self.color_grey+s+self.color_reset
    def yellow(self,s): return self.color_yellow+s+self.color_reset
    def red(self,s): return self.color_red+s+self.color_reset
    def bold_red(self,s): return self.color_bold_red+s+self.color_reset
    def blue_underline(self,s): return self.color_blue_underline+s+self.color_reset
ansi=Ansi()


# [custom logger formatter](https://stackoverflow.com/questions/384076/how-can-i-color-python-logging-output)
class CustomFormatter(logging.Formatter):
    def get_fmt(color):
        return '['+color('%(levelname)s')+'] %(message)s'

    FORMATS = {
        logging.DEBUG: get_fmt(ansi.grey),
        logging.INFO: get_fmt(ansi.blue),
        logging.WARNING: '['+ansi.yellow('WARN')+'] %(message)s',
        logging.ERROR: get_fmt(ansi.red),
        logging.CRITICAL: get_fmt(ansi.bold_red)
    }

    def format(self, record):
        log_fmt = self.FORMATS.get(record.levelno)
        formatter = logging.Formatter(log_fmt)
        return formatter.format(record)
log=logging.getLogger('pyjudge')


class Runner:
    def run(self,cmd:list,inp:Path,outp:Path) -> tuple[bool,float]:
        if outp and not outp.exists(): outp.touch()
        try:
            start=time.time()
            subprocess.run(
                cmd,stdin=inp.open('r'),
                stdout=outp.open('w'),
                # stderr=outp.open('w'),
                timeout=self.tle,
                check=True
            )
            end=time.time()
        except subprocess.CalledProcessError as err:
            log.warning(inp.name+' '+ansi.magenta('Runtime Error')+' '+str(err))
            return False,0
        except subprocess.TimeoutExpired as err:
            log.warning(inp.name+' '+ansi.blue('Time Limit Exceeded'))
            return False,self.tle
        except Exception as err:
            log.error(inp.name+' '+ansi.bold_red('Unknown Error')+' '+str(err))
            return False,0
        else:
            return True,end-start

    def __init__(self,tle:float) -> None:
        self.tle=tle


class Judger:
    def compare(self,ansp:Path,outp:Path) -> tuple[bool,int,str,str]:
        with ansp.open() as ansf, outp.open() as outf:
            line=0
            while True:
                line+=1
                out=outf.readline().split()
                ans=ansf.readline().split()
                if out != ans:
                    while len(out) < len(ans): out.append('')
                    while len(ans) < len(out): ans.append('')
                    for i,j in zip(ans,out):
                        if i!=j:
                            return False,line,i,j
                if not out or not ans: break
        return True,0,'',''

    def __init__(self) -> None:
        pass


class Tester:
    def get_testcases(self,dir:Path,save_output:bool,check_exist:bool) -> list[Path]:
        def check_decode(file:Path):
            try:
                with file.open() as f: f.readline()
            except UnicodeDecodeError as err:
                log.error(file.name+' '+ansi.bold_red('unknown file encoding'))
                return False
            return True

        res=[]
        for inp in dir.rglob('*.in'):
            if not check_decode(inp): continue
            outp=inp.parent.joinpath(inp.stem+'.out')
            if save_output:
                if not outp.exists(): outp.touch()
                res.append(inp)
            elif check_exist:
                if outp.exists() and check_decode(outp): res.append(inp)
            else:
                res.append(inp)
        return res

    def run(self,cmd:list,dir:Path,save_output:bool) -> tuple[int,int,float]:
        runner=Runner(self.tle)
        data=self.get_testcases(dir,save_output,False)
        ok,tot,slowest=0,len(data),0
        for inp in data:
            outp=inp.parent.joinpath(inp.stem+'.out')
            good,time_used=runner.run(cmd,inp,outp if save_output else Path(os.devnull))
            if not good: continue
            log.info(
                '%(input)s %(verdict)s, Executed in %(time)s ms'% {
                    'input':inp.name,
                    'verdict':ansi.green('Finished'),
                    'time':ansi.green(str(int(time_used*1000)))
                }
            )
            ok+=1
            slowest=max(slowest,time_used)
        return ok,tot,slowest

    def judge(self,cmd:list,outp:Path,dir:Path) -> tuple[int,int,float]:
        runner=Runner(self.tle)
        judger=Judger()
        data=self.get_testcases(dir,False,True)
        ok,tot,slowest=0,len(data),0
        for inp in data:
            good,time_used=runner.run(cmd,inp,outp)
            if not good: continue
            ansp=inp.parent.joinpath(inp.stem+'.out')

            ac,line,ans,out=judger.compare(ansp,outp)
            if ac:
                log.info(
                    '%(input)s %(verdict)s, Executed in %(time)s ms'% {
                        'input':inp.name,
                        'verdict':ansi.green('Accepted'),
                        'time':ansi.green(str(int(time_used*1000)))
                    }
                )
            else:
                log.warning(
                    '%(input)s %(verdict)s on line %(line)s: "%(ans)s" <-> "%(out)s", Executed in %(time)s ms'% {
                        'input':inp.name,
                        'verdict':ansi.red('Wrong Answer'),
                        'line':ansi.red(str(line)),
                        'ans':ansi.green(ans),
                        'out':ansi.red(out),
                        'time':ansi.red(str(int(time_used*1000)))
                    }
                )
            ok+=ac
            slowest=max(slowest,time_used)
        return ok,tot,slowest

    def hack(self,test:list,std:list,gen:list,inp:Path,ansp:Path,outp:Path) -> bool:
        cnt=0
        runner=Runner(self.tle)
        judger=Judger()
        slash=['/','-','\\','|']
        while True:
            cnt+=1
            print('[ %(x)s ] Hacking on testcase #%(y)s...'
                  %{'x':slash[cnt%len(slash)],'y':str(cnt)},end='\r')
            if not runner.run(gen,Path(os.devnull),inp)[0]: return False
            if not runner.run(std,inp,ansp)[0]: return False
            if not runner.run(test,inp,outp)[0]: return True
            ac,line,ans,out=judger.compare(ansp,outp)
            if not ac:
                log.warning(
                    '%(verdict)s on line %(line)s: "%(ans)s" <-> "%(out)s"'% {
                        'verdict':ansi.red('Wrong Answer'),
                        'line':ansi.red(str(line)),
                        'ans':ansi.green(ans),
                        'out':ansi.red(out),
                    }
                )
                return True

    def __init__(self,tle:float) -> None:
        self.tle=tle


def main():
    parser=argparse.ArgumentParser(
        prog='pyjudge',
        description='A simple competitive programming judger',
        epilog='examples: https://github.com/HEltim7/cplib/blob/master/Tools/pyjudge.md'
    )
    parser.add_argument('-v','--version',action='version',version='%(prog)s 1.0.3 by HEltim7')
    parser.add_argument('--debug',help='enable debugging',action='store_true')
    subparsers=parser.add_subparsers(dest='subparser',help='action')

    judge_parser=subparsers.add_parser('judge',help='run and judge code by given testcases')
    judge_parser.add_argument('filename',help='the Code to judge')
    judge_parser.add_argument('-t','--tle',type=float,default=1,help='time limit per testcase, default is 1 second')
    judge_parser.add_argument('-d','--dir',default='data',help='the dirctory of testcases, default is "data"')

    run_parser=subparsers.add_parser('run',help='run code and save output')
    run_parser.add_argument('filename',help='the code to run')
    run_parser.add_argument('-s','--save',action='store_true',help='save output as .out file')
    run_parser.add_argument('-t','--tle',type=float,default=1,help='time limit per testcase, default is 1 second')
    run_parser.add_argument('-d','--dir',default='data',help='the dirctory of testcases, default is "data"')

    hack_parser=subparsers.add_parser('hack',help='brute-force search for a hack testcase')
    hack_parser.add_argument('filename',help='the code to hack')
    hack_parser.add_argument('-s','--std',help='standard solution code',required=True)
    hack_parser.add_argument('-g','--gen',help='testcase generator',required=True)
    hack_parser.add_argument('-t','--tle',type=float,default=1,help='time limit per testcase, default is 1 second')
    hack_parser.add_argument('-d','--dir',default='data',help='the dirctory of testcases, default is "data"')

    args=parser.parse_args()
    action=args.subparser
    tester=Tester(args.tle)
    global _DEBUG
    _DEBUG=args.debug

    handler=logging.StreamHandler()
    handler.setLevel(logging.DEBUG)
    handler.setFormatter(CustomFormatter())
    log.setLevel(logging.INFO)
    log.addHandler(handler)

    cpp_compile_flags=['-std=c++20','-O2','-DONLINE_JUDGE']
    cpp_compiler=['g++']
    c_compile_flags=['-std=c11','-O2','-DONLINE_JUDGE']
    c_compiler=['gcc']
    python_interpreter=['python']

    work_dir=tempfile.TemporaryDirectory(prefix='pyjudge_')

    def work_path(file:str) -> Path:
        return Path(work_dir.name).joinpath(file)

    def get_exe(file:str,bin_name:str) -> list:
        filep=Path(file)
        if filep.suffix in ['.cpp','.cc','.cxx','.c++','.cplusplus','.c']:
            exe=work_path(bin_name)
            log.info('Compiling '+file+'...')
            try:
                if filep.suffix not in ['.c']:
                    subprocess.run(cpp_compiler+cpp_compile_flags+[file,'-o',exe],check=True)
                else:
                    subprocess.run(c_compiler+c_compile_flags+[file,'-o',exe],check=True)
            except subprocess.CalledProcessError as err:
                log.error(ansi.bold_red('Compile Error'))
                return []
            return [exe]
        elif filep.suffix in ['.py']:
            return python_interpreter+[filep]
        else:
            log.error(ansi.bold_red('Unknown File Type'))
        return []

    if action=='judge':
        cmd=get_exe(args.filename,'test.bin')
        if cmd==[]: return

        log.info('Running...')
        outp=work_path('test.out')
        accepted,total,slowest=tester.judge(cmd,outp,Path(args.dir))

        log.info('')
        log.info('Accepted '+str(accepted)+' / '+str(total))
        log.info('Slowest: '+ansi.green(str(int(slowest*1000)))+' ms')
    elif action=='run':
        cmd=get_exe(args.filename,'test.bin')
        if cmd==[]: return

        log.info('Running...')
        finished,total,slowest=tester.run(cmd,Path(args.dir),args.save)

        log.info('')
        log.info('Finished '+str(finished)+' / '+str(total))
        log.info('Slowest: '+ansi.green(str(int(slowest*1000)))+' ms')
    elif action=='hack':
        test=get_exe(args.filename,'test.bin')
        if test==[]: return
        std=get_exe(args.std,'std.bin')
        if std==[]: return
        gen=get_exe(args.gen,'gen.bin')
        if gen==[]: return
        
        inp=work_path('hack.in')
        ansp=work_path('hack.out')
        outp=work_path('test.out')
        if tester.hack(test,std,gen,inp,ansp,outp):
            dir=Path(args.dir)
            if not dir.exists(): dir.mkdir()
            for i in itertools.count():
                hack_in=dir.joinpath('hack_'+str(i+1)+'.in')
                hack_out=dir.joinpath(hack_in.stem+'.out')
                if not hack_in.exists():
                    shutil.copyfile(inp,hack_in)
                    shutil.copyfile(ansp,hack_out)
                    break
            
            log.info('')
            log.info(ansi.green('Hacking success')+
                     ', testcase has save to '+ansi.blue_underline(str(hack_in)))
        else:
            log.info('')
            log.warning(ansi.red('Hacking failed'))

    work_dir.cleanup()

if __name__ == '__main__':
    try:
        start=time.time()
        main()
        end=time.time()
    except KeyboardInterrupt:
        log.warning('KeyboardInterrupt')
    except Exception as err:
        if _DEBUG: traceback.print_exc()
        log.error(ansi.bold_red('Unknown Error')+' '+str(err))
        exit(1)
    else: log.info('Finished in %.3f s\n'%(end-start))