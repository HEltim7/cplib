import os
import sys
import time
import shutil
import logging
import argparse
import tempfile
import itertools
import threading
import traceback
import subprocess

from enum import Enum,auto
from pathlib import Path

PYJUDGE_DEBUG=False
PYJUDGE_SHOW_STDERR=True

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


class Verdict(Enum):
    ACCEPTED                  = auto()
    FINISHED                  = auto()
    WRONG_ANSWER              = auto()
    COMPILE_ERROR             = auto()
    RUNTIME_ERROR             = auto()
    TIME_LIMIT_EXCEEDED       = auto()
    MEMORY_LIMIT_EXCEEDED     = auto()

    UNKONWN_ERROR             = auto()
    UNKONWN_FILE_TYPE         = auto()
    UNKONWN_FILE_ENCODING     = auto()
    NO_SUCH_FILE_OR_DIRECTORY = auto()

    def format(self) -> str:
        match self:
            case Verdict.ACCEPTED: return ansi.green('Accepted')
            case Verdict.FINISHED: return ansi.green('Finished')
            case Verdict.WRONG_ANSWER: return ansi.red('Wrong Answer')
            case Verdict.RUNTIME_ERROR: return ansi.magenta('Runtime Error')
            case Verdict.COMPILE_ERROR: return ansi.bold_red('Compile Error')
            case Verdict.TIME_LIMIT_EXCEEDED: return ansi.blue('Time Limit Exceeded')
            case Verdict.MEMORY_LIMIT_EXCEEDED: return ansi.blue('Memory Limit Exceeded')

            case Verdict.UNKONWN_FILE_TYPE: return ansi.bold_red('Unkonwn File Type')
            case Verdict.UNKONWN_FILE_ENCODING: return ansi.bold_red('Unkonwn File Encoding')
            case Verdict.NO_SUCH_FILE_OR_DIRECTORY: return ansi.bold_red('No Such File Or Directory')
            case _: return ansi.bold_red('Unkonwn Error')


class Result:
    def __init__(self,verdict:Verdict,message:str='',time_used:float=0,memory_used:int=0):
        self.verdict = verdict
        self.message = message
        self.time_used = time_used
        self.memory_used = memory_used

    def __iter__(self):
        yield self.verdict
        yield self.message
        yield self.time_used
        yield self.memory_used

    def good(self) -> bool:
        return self.verdict == Verdict.ACCEPTED or self.verdict == Verdict.FINISHED

    def format(self) -> str:
        def toMilli():
            return str(int(self.time_used*1000))

        res = self.verdict.format()
        if self.message != '':
            res += ' ' + self.message.replace('\n',' ')
        match self.verdict:
            case Verdict.ACCEPTED | Verdict.FINISHED:
                res += ' Executed in %s ms'% ansi.green(toMilli())
            case Verdict.WRONG_ANSWER:
                res += ' Executed in %s ms'% ansi.red(toMilli())
        return res


class FileCtrl:
    work_dir=tempfile.TemporaryDirectory(prefix='pyjudge_')
    
    @staticmethod
    def workPath(filename:str) -> Path:
        return Path(FileCtrl.work_dir.name).joinpath(filename)

    @staticmethod
    def getTestcases(dir:Path,touch_out:bool) -> list[tuple[Path,Path]]:
        res=[]
        for inp in dir.rglob('*.in'):
            outp=inp.parent.joinpath(inp.stem+'.out')
            if not outp.exists():
                if touch_out:
                    outp.touch()
                    res.append((inp,outp))
                else: res.append((inp,None))
            else: res.append((inp,outp))
        return res

    @staticmethod
    def findAvailableId(dir:Path,prefix:str,suffix:str) -> int:
        if not dir.exists(): dir.mkdir(parents=True)
        for i in itertools.count():
            if not dir.joinpath(prefix+str(i+1)+suffix).exists():
                return i+1

    @staticmethod
    def copyFile(source:Path,target:Path) -> None:
        if not target.exists(): target.touch()
        shutil.copyfile(source,target)

    @staticmethod
    def mergeFile(x:Path,y:Path,filename:str) -> Path:
        ls = [x, y]
        res = FileCtrl.workPath(filename)
        with res.open('w') as out:
            for i in ls:
                with i.open('r') as f:
                    for line in f:
                        out.write(line)
                    out.write('\n')
        return res
    
    @staticmethod
    def fileToStr(inp:Path) -> str:
        with inp.open('r') as f:
            return f.read()

    @staticmethod
    def clearup():
        FileCtrl.work_dir.cleanup()


class Compiler:
    cpp_compiler       = ['g++']
    cpp_compile_flags  = ['-std=c++20','-O2','-DONLINE_JUDGE']
    c_compiler         = ['gcc']
    c_compile_flags    = ['-std=c11','-O2','-DONLINE_JUDGE']
    python_interpreter = ['python']
    python_flags       = []

    def run(self,cmd:list) -> Result:
        try:
            subprocess.run(cmd,check=True)
        except subprocess.CalledProcessError as err:
            return Result(Verdict.COMPILE_ERROR)
        return Result(Verdict.FINISHED)

    def compileCpp(self,inp:Path,outp:Path) -> Result:
        return self.run(self.cpp_compiler+self.cpp_compile_flags+[inp,'-o',outp])

    def compileC(self,inp:Path,outp:Path) -> Result:
        return self.run(self.c_compiler+self.c_compile_flags+[inp,'-o',outp])

    def processPython(self,inp:Path) -> list:
        return self.python_interpreter + self.python_flags + [inp]

    def process(self,inp:Path,name:str) -> tuple[Result,list]:
        outp=FileCtrl.workPath(name)
        match inp.suffix:
            case '.cpp' | '.cc' | '.cxx' | '.c++' | '.cplusplus':
                return self.compileCpp(inp,outp),outp
            case '.c':
                return self.compileC(inp,outp),outp
            case '.py':
                return Result(Verdict.FINISHED),self.processPython(inp)
            case _:
                return Result(Verdict.UNKONWN_FILE_TYPE),[]

    def compileable(self,inp:Path) -> bool:
        return inp.suffix in ['.cpp','.cc','.cxx','.c++','.cplusplus','.c']

    # def __init__(self,
    #     cpp_compiler:list,cpp_compile_flags:list,
    #     c_compiler:list,c_compile_flags:list,
    #     python_interpreter:list,python_flags:list
    # ):
    #     self.cpp_compiler       = cpp_compiler
    #     self.cpp_compile_flags  = cpp_compile_flags
    #     self.c_compiler         = c_compiler
    #     self.c_compile_flags    = c_compile_flags
    #     self.python_interpreter = python_interpreter
    #     self.python_flags       = python_flags

    def __init__(self):
        pass


class Runner:
    def run(self,cmd:list,inp:Path,outp:Path,spj:bool=False) -> Result:
        try:
            with inp.open() as f: f.readline()
        except UnicodeDecodeError as err:
            return Result(Verdict.UNKONWN_FILE_ENCODING)

        if outp and not outp.exists(): outp.touch()
        try:
            start=time.time()
            subprocess.run(
                cmd,
                timeout=self.tle,
                check=True,
                stdin=inp.open('r'),
                stdout=outp.open('w'),
                stderr=sys.stderr if PYJUDGE_SHOW_STDERR
                    else Path(os.devnull).open('w')
            )
            end=time.time()
        except subprocess.CalledProcessError as err:
            if spj and err.returncode == 1:
                return Result(Verdict.WRONG_ANSWER,FileCtrl.fileToStr(outp))
            return Result(Verdict.RUNTIME_ERROR,str(err))
        except subprocess.TimeoutExpired as err:
            return Result(Verdict.TIME_LIMIT_EXCEEDED,'',self.tle)
        except Exception as err:
            print(str(err))
            return Result(Verdict.UNKONWN_ERROR,str(err))
        else:
            if spj:
                return Result(Verdict.ACCEPTED,FileCtrl.fileToStr(outp))
            return Result(Verdict.FINISHED,'',end-start)

    def specialRun(self,cmd:list,inp:Path,outp:Path) -> Result:
        return self.run(cmd,inp,outp,True)

    def __init__(self,tle:float) -> None:
        self.tle=tle


class Judger:
    def waResult(self,line:int,ans:str,out:str) -> Result:
        return Result(
            Verdict.WRONG_ANSWER,
            'on line %(line)d: "%(ans)s" <-> "%(out)s"'% {
                'line': line,
                'ans': ansi.green(ans),
                'out': ansi.red(out)
            }
        )

    def compare(self,ansp:Path,outp:Path) -> Result:
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
                        if i!=j: return self.waResult(line,i,j)
                if not out or not ans: break
        return Result(Verdict.ACCEPTED)

    def specialJudge(self,spj:list,inp:Path,outp:Path) -> Result:
        in_out = FileCtrl.mergeFile(inp,outp,'in_out.txt')
        res = Runner(60).specialRun(spj,in_out,FileCtrl.workPath('verdict.txt'))
        return res

    def __init__(self) -> None:
        pass


class Tester:
    def run(self,cmd:list,dir:Path,save_output:bool) -> tuple[int,int,float]:
        log.info('Running...')
        runner=Runner(self.tle)
        data=FileCtrl.getTestcases(dir,save_output)
        ok,tot,slowest=0,len(data),0
        for inp,outp in data:
            res=runner.run(
                cmd,inp,outp if save_output else Path(os.devnull)
            )
            if res.verdict != Verdict.FINISHED:
                log.warning(inp.name+' '+res.format())
            else:
                log.info(inp.name+' '+res.format())
                ok+=1
                slowest=max(slowest,res.time_used)

        log.info('')
        log.info('Passed '+str(ok)+' / '+str(tot))
        log.info('Slowest: '+ansi.green(str(int(slowest*1000)))+' ms')

    def judge(self,dir:Path,test:list,spj:list=None) -> None:
        log.info('Running...')
        judger=Judger()
        runner=Runner(self.tle)
        outp=FileCtrl.workPath('test.out')
        data=FileCtrl.getTestcases(dir,False)
        ok,tot,slowest=0,len(data),0
        
        for inp,ansp in data:
            res=runner.run(test,inp,outp)
            if not res.good():
                log.warning(inp.name+' '+res.format())
                continue

            if not spj:
                if ansp:
                    tmp=judger.compare(ansp,outp)
                    res=Result(tmp.verdict,tmp.message,res.time_used,res.memory_used)
            else:
                res=judger.specialJudge(spj,inp,outp)

            if res.good():
                ok+=1
                log.info(inp.name+' '+res.format())
            elif spj and res.verdict != Verdict.WRONG_ANSWER:
                log.warning(inp.name+' '+ansi.bold_red('Special Judge Error:')+' '+res.format())
            else:
                log.warning(inp.name+' '+res.format())
            
            slowest=max(slowest,res.time_used)
        
        log.info('')
        log.info('Passed '+str(ok)+' / '+str(tot))
        log.info('Slowest: '+ansi.green(str(int(slowest*1000)))+' ms')

    def hack(self,dir:Path,test:list,gen:list,std:list=None,spj:list=None) -> bool:
        inp = FileCtrl.workPath('hack.in')
        ansp = FileCtrl.workPath('hack.out')
        outp = FileCtrl.workPath('test.out')

        class Status:
            cnt = 0
            gen_err = False
            std_err = False
            spj_err = False
            res = Result(Verdict.FINISHED)

        def hack_impl(st:Status) -> None:
            runner=Runner(self.tle)
            judger=Judger()
            while True:
                st.cnt+=1
                
                st.res=runner.run(gen,Path(os.devnull),inp)
                if not st.res.good(): 
                    st.gen_err=True
                    return
                
                st.res=runner.run(test,inp,outp)
                if not st.res.good(): return
                
                if std:
                    st.res=runner.run(std,inp,ansp)
                    if not st.res.good():
                        st.std_err=True
                        return

                    tmp=judger.compare(ansp,outp)
                    if not tmp.good():
                        st.res=Result(
                            tmp.verdict,tmp.message,st.res.time_used,st.res.memory_used
                        )
                        return
                else:
                    st.res=judger.specialJudge(spj,inp,outp)
                    if not st.res.good():
                        if st.res.verdict != Verdict.WRONG_ANSWER:
                            st.spj_err=True
                        return

        st=Status()
        hackt=threading.Thread(target=hack_impl,args=[st],daemon=True)
        hackt.start()

        def get(x:int):
            if x==0: return "[    ] "
            if x==1: return "[>   ] "
            if x==2: return "[>>  ] "
            if x==3: return "[>>> ] "
            if x==4: return "[ >>>] "
            if x==5: return "[  >>] "
            if x==6: return "[   >] "

        for i in itertools.count():
            x=(i+1)%700
            time.sleep(0.001)
            if not hackt.is_alive(): break
            print(get(x//100)+"hacking on testcase #"+str(st.cnt)+"...",end='\r')

        if st.gen_err:
            log.warning(ansi.bold_red('Generator Error:' + ' ' + st.res.format()))
        elif st.std_err:
            log.warning(ansi.bold_red('Standard Solution Error:' + ' ' + st.res.format()))
        elif st.spj_err:
            log.warning(ansi.bold_red('Special Judge Error:' + ' ' + st.res.format()))
        else:
            log.info(st.res.format())
        
        log.info('')
        if st.gen_err or st.std_err or st.spj_err:
            log.info(ansi.bold_red('Hacking Failed'))
        else:
            log.info(ansi.green('Hacking Success'))
            id=FileCtrl.findAvailableId(dir,'hack_','.in')
            if std: id=max(id,FileCtrl.findAvailableId(dir,'hack_','.out'))

            in_target=dir.joinpath('hack_%d.in'%id)
            FileCtrl.copyFile(inp,in_target)
            log.info('testcase has been saved to:')
            log.info('file://'+str(in_target.absolute()))

            if std:
                out_target=dir.joinpath('hack_%d.out'%id)
                FileCtrl.copyFile(ansp,out_target)
                log.info('file://'+str(out_target.absolute()))
            log.info('')


    def __init__(self,tle:float) -> None:
        self.tle=tle


def main():
    handler=logging.StreamHandler()
    handler.setLevel(logging.DEBUG)
    handler.setFormatter(CustomFormatter())
    log.setLevel(logging.INFO)
    log.addHandler(handler)

    parser=argparse.ArgumentParser(
        prog='pyjudge',
        description='A simple competitive programming judger',
        epilog='examples: https://github.com/HEltim7/cplib/blob/master/Tools/pyjudge.md'
    )
    parser.add_argument('-v','--version',action='version',version='%(prog)s 1.0.4 by HEltim7')
    parser.add_argument('--debug',help='enable debugging',action='store_true')
    parser.add_argument('--hide-stderr',action='store_false',help='suppress printing stderr')
    parser.add_argument('--cpp-compiler',help='specify the compiler for C++')
    parser.add_argument('--cpp-flags',help='compile flags for C++')
    parser.add_argument('--py-interpreter',help='specify the interpreter for Python')
    parser.add_argument('--py-flags',help='interpreter flags for Python')
    subparsers=parser.add_subparsers(dest='subparser',help='action')

    judge_parser=subparsers.add_parser('judge',help='run and judge code by given testcases')
    judge_parser.add_argument('filename',type=Path,help='the Code to judge')
    judge_parser.add_argument('-t','--tle',type=float,default=1,help='time limit per testcase, default is 1 second')
    judge_parser.add_argument('-d','--dir',type=Path,default='data',help='the dirctory of testcases, default is "./data"')
    judge_parser.add_argument('--spj',type=Path,help='enable special judge')

    run_parser=subparsers.add_parser('run',help='run code and save output')
    run_parser.add_argument('filename',type=Path,help='the code to run')
    run_parser.add_argument('-t','--tle',type=float,default=1,help='time limit per testcase, default is 1 second')
    run_parser.add_argument('-d','--dir',type=Path,default='data',help='the dirctory of testcases, default is "./data"')
    run_parser.add_argument('-s','--save',action='store_true',help='save output as .out file')

    hack_parser=subparsers.add_parser('hack',help='brute-force search for a hack testcase')
    hack_parser.add_argument('filename',type=Path,help='the code to hack')
    hack_parser.add_argument('-t','--tle',type=float,default=1,help='time limit per testcase, default is 1 second')
    hack_parser.add_argument('-d','--dir',type=Path,default='data',help='the dirctory of testcases, default is "./data"')
    hack_parser.add_argument('-g','--gen',type=Path,help='testcase generator',required=True)
    exgroup=hack_parser.add_mutually_exclusive_group(required=True)
    exgroup.add_argument('-s','--std',type=Path,help='standard solution code')
    exgroup.add_argument('--spj',type=Path,help='special judge code')

    args=parser.parse_args()
    action=args.subparser
    tester=Tester(args.tle)
    compiler = Compiler()

    global PYJUDGE_DEBUG
    PYJUDGE_DEBUG=args.debug
    global PYJUDGE_SHOW_STDERR
    PYJUDGE_SHOW_STDERR=args.hide_stderr

    def prepare(file:Path,name:str) -> list:
        if not file.exists():
            log.error(file.name + ' ' + Verdict.NO_SUCH_FILE_OR_DIRECTORY.format())
            return
        if compiler.compileable(file):
            log.info('Compiling '+file.name+'...')
        res,exe=compiler.process(file,name)
        if not res.good():
            log.error(file.name + ' ' + res.format())
        else: return exe

    if action == 'judge':
        test = prepare(args.filename, 'test.bin')
        spj = None
        if not test: return
        if args.spj:
            spj = prepare(args.spj, 'spj.bin')
            if not spj: return
        tester.judge(args.dir, test, spj)

    elif action=='run':
        test=prepare(args.filename,'test.bin')
        if not test: return
        tester.run(test,args.dir,args.save)
    elif action=='hack':
        test=prepare(args.filename,'test.bin')
        if not test: return
        gen=prepare(args.gen,'gen.bin')
        if not gen: return

        std,spj=None,None
        if args.std:
            std=prepare(args.std,'std.bin')
            if not std: return
        if args.spj:
            spj=prepare(args.spj,'spj.bin')
            if not spj: return
        tester.hack(args.dir,test,gen,std,spj)


if __name__ == '__main__':
    try:
        start=time.time()
        main()
        FileCtrl.clearup()
        end=time.time()
    except KeyboardInterrupt:
        log.warning('KeyboardInterrupt')
    except Exception as err:
        if PYJUDGE_DEBUG: traceback.print_exc()
        log.error(ansi.bold_red('Unknown Error')+' '+str(err))
        exit(1)
    else: log.info('Finished in %.3f s\n'%(end-start))
