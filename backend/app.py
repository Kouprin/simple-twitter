from __future__ import print_function
from flask import Flask, request, render_template
import socket
import requests
import sys

def eprint(*args, **kwargs):
    print(*args, file=sys.stderr, **kwargs)

app = Flask(__name__)

def engine_query(query, data):
    sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    server_address = ('messenger', 8888)
    sock.connect(server_address)
    send_it = query + '\t' + '\t'.join(data)
    eprint(send_it.encode('ASCII'))
    sock.sendall(send_it.encode('ASCII'))
    reply = sock.recv(4096)
    sock.close()
    eprint(reply)
    reply = reply.decode('ASCII')
    eprint('getting ' + reply)
    if not reply or reply == "NO":
        return None
    return reply.split('\t')[1:]

@app.route('/')
def hello_world():
    return 'Hello, World!'

@app.route('/channel')
def link_parse():
    link = request.args.get('link')
    channel = engine_query('getChannelByLink', [link])
    if not channel:
        return render_template('channel.html', head='No such channel', data='')
    
    eprint(channel[0])
    eprint(channel[0].encode('ASCII'))
    messages = engine_query('getMessages', [channel[0]])
    return render_template('channel.html', head=channel[1], data=messages)

if __name__ == '__main__':
    app.run(debug=True, host='0.0.0.0')
