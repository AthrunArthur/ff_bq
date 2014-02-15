import os
import json
import subprocess
import smtplib
import sys
from email.mime.text import MIMEText


msg_num = 100000000
max_thrd_num = 8

def execute_cmd(cmd):
	p = subprocess.Popen(cmd, shell=True, stdout = subprocess.PIPE)
	return p.stdout.read()
      

def gen_cmd(is_ff, thrd_num):
  s = './build/ff_bq '
  if is_ff:
    s += '--ff-lock '
  else:
    s += '--std-lock '
    
  s += '--msg-num=' + str(msg_num) + ' '
  
  s += '--threads=' + str(thrd_num)
  
  return s


def run_and_get_time(is_ff, thrd_num):
  
  s = gen_cmd(is_ff, thrd_num)
  print 'running ' + s
  i = 0
  max = 3
  res = 0
  while i < max:
    i += 1
    execute_cmd(s)
    ts = open('time.json', 'r').read()
    tr = json.loads(ts)
    res += int(tr['time'])
  res = res/max
  return res

sender = 'arthurathrun@gmail.com'
recver = 'athrunarthur@gmail.com'
passwd = 'athrun911500'

def send_to_AA():
  print 'sending email...'
  fp = 'final.json'
  if not os.path.exists(fp):
    body = 'Cannot find file final.json'
  else:
    body = open(fp, 'r').read()
    
  headers = ['From: ' + sender,
	     'Subject: ' + 'Auto-Send: BQ result for FF',
	     'To: ' + recver,
	     'MIME-Version: 1.0',
	     'Content-Type: text/html']
  headers = '\r\n'.join(headers)
  
  msg = headers + '\r\n\r\n' + body
  
  try:
    s = smtplib.SMTP('smtp.gmail.com', 587)
    print 'connected'
    s.ehlo()
    s.starttls()
    s.ehlo()
    print 'login...'
    s.login('arthurathrun@gmail.com', 'athrun911500')
    print 'sending...'
    s.sendmail('arthurathrun@gmail.com',['athrunarthur@gmail.com'], msg)
    s.quit()
    print 'send email done'
  except :
    print 'error: unable to send email!'
    print sys.exc_info()[0]
    

  
def run_benchmark():
  
  i = 0
  max = max_thrd_num
  res = {'ff':{}, 'std':{}}
  
  while i < max:
    i += 1
    res['ff'][i] = run_and_get_time(True, i)
    res['std'][i] = run_and_get_time(False, i)
    
  print res
  
  open('final.json', 'w').write(json.dumps(res))
  
if __name__ == '__main__':

  run_benchmark()
  
  send_to_AA()
  