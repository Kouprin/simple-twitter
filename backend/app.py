from __future__ import print_function
from flask import Flask, request, render_template, session, redirect, url_for
from flask_login import LoginManager, login_user, login_required, logout_user
from forms import SignupForm, LoginForm
from user import User
import socket
import requests
import sys

def eprint(*args, **kwargs):
    print(*args, file=sys.stderr, **kwargs)

app = Flask(__name__)
app.secret_key = b'(YG&RW9ywgrfwi6gf86srGEFWI^sfuydafgisdfig123'
login_manager = LoginManager()
login_manager.init_app(app)

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
    if reply == "OK":
        return True
    return reply.split('\t')[1:]

@login_manager.user_loader
def load_user(email):
    user = engine_query('getUser', [email])
    if not user:
        return None
    return User(name=user[1], email=email)

@app.route('/protected')
@login_required
def protected():
    return "protected area"

@app.route('/signup', methods=['GET', 'POST'])
def signup():
    form = SignupForm()
    if request.method == 'GET':
        return render_template('signup.html', form=form)
    elif request.method == 'POST':
        if form.validate_on_submit():
            if load_user(form.email.data):
                return "Email address already exists" # TODO popup?
            else:
                if engine_query('addUser', [form.name.data, form.email.data, form.password.data]):
                    user = load_user(form.email.data)
                    login_user(user)
                    return "User created"
                else:
                    return "Server failed"
        else:
            return "Form didn't validate" # TODO popup?

@app.route('/login', methods=['GET','POST'])
def login():
    form = LoginForm()
    if request.method == 'GET':
        return render_template('login.html', form=form)
    elif request.method == 'POST':
        if form.validate_on_submit():
            user = load_user(form.email.data)
            if user:
                if engine_query('loginUser', [form.email.data, form.password.data]):
                    login_user(user)
                    return "User logged in"                
                else:
                    return "Wrong password"            
            else:
                return "User doesn't exist"        
        else:
            return "Form didn't validate" # TODO popup?

@app.route("/logout")
@login_required
def logout():
    logout_user()
    return "Logged out"

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
