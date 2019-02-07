class User:
    def __init__(self, id, name, email):
        self.id = id
        self.name = name
        self.email = email
    def __repr__(self):
        return '<User %r>' % self.email
    def is_authenticated(self):
        return True
    def is_active(self):
        return True
    def is_anonymous(self):
        return False
    def get_id(self):
        return str(self.email)
