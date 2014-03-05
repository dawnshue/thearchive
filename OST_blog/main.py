import os
import urllib

from google.appengine.api import users
from google.appengine.ext import ndb

import jinja2
import webapp2

from models import *

JINJA_ENVIRONMENT = jinja2.Environment(
    loader=jinja2.FileSystemLoader(os.path.dirname(__file__)),
    extensions=['jinja2.ext.autoescape'],
    autoescape=True)

#class MainPage(webapp2.RequestHandler):
def header(self):
    #def header(self):
	user_on = 0
        if users.get_current_user():
	    user_on = 1
            login_url = users.create_logout_url(self.request.uri)
            login_text = 'Log Out'
	    welcome = str(users.get_current_user())
        else:
            login_url = users.create_login_url(self.request.uri)
            login_text = 'Log In'
	    welcome = 'Welcome!'
	    manage_text = ''

	template_values = {}
        template_values['login_url'] = login_url
        template_values['login_text'] = login_text
	template_values['welcome'] = welcome
	template_values['user_on'] = user_on

        header = JINJA_ENVIRONMENT.get_template('temp_header.html')
        self.response.write(header.render(template_values))

def footer(self):
    #def footer(self):
	footer = JINJA_ENVIRONMENT.get_template('temp_footer.html')
	self.response.write(footer.render())

class MainPage(webapp2.RequestHandler):

    def get(self, page=None):	
	header(self)

	if page:
	  entries = Entry.get_some(int(page))
	  older_url = "/p"+str((int(page)+1))
	else:
	  page = str(0)
	  entries = Entry.get_some(1)
	  older_url = "/p2"
	heading = 'Latest Posts'
	temp_vals = { 'heading' : heading,
			'page' : page,
			'entries' : entries,
			'older_url' : older_url
			}

	display = JINJA_ENVIRONMENT.get_template('temp_display.html')
	self.response.write(display.render(temp_vals))

	footer(self)

class BlogHandler(webapp2.RequestHandler):
    def get(self, author, blog, page=None):
	header(self)

	if page:
	  entries = Entry.get_author_blog(author, blog, int(page))
	  older_url = "/"+author+"/blog="+blog+"/p"+str((int(page)+1))
	else:
	  page = str(0)
	  entries = Entry.get_author_blog(author, blog, 1)
	  older_url = "/"+author+"/blog="+blog+"/p2"

	heading = blog
	temp_vals = { 'heading' : heading,
			'page' : page,
			'entries' : entries,
			'older_url' : older_url
			}

	display = JINJA_ENVIRONMENT.get_template('temp_display.html')
	self.response.write(display.render(temp_vals))

	self.response.write('<p class="main"><a href="/rss/user=')
	self.response.write(author+'/blog='+blog+'/rss.xml">Get RSS Feed</a>')
	footer(self)

class UserHandler(webapp2.RequestHandler):
    def get(self, author, page=None):
	header(self)

	if page:
	  entries = Entry.get_author_some(author, int(page))
	  older_url = "/"+author+"/p"+str((int(page)+1))
	else:
	  page = str(0)
	  entries = Entry.get_author_some(author, 1)
	  older_url = "/"+author+"/p2"

	heading = 'Posts by '+author
	temp_vals = { 'heading' : heading,
			'page' : page,
			'entries' : entries,
			'older_url' : older_url
			}

	display = JINJA_ENVIRONMENT.get_template('temp_display.html')
	self.response.write(display.render(temp_vals))

	footer(self)

class TagHandler(webapp2.RequestHandler):
    def get(self, tag, page=None):
	header(self)

	if page:
	  entries = Entry.get_tagged_some(tag, int(page))
	  older_url = "/tagged/"+tag+"/p"+str((int(page)+1))
	else:
	  page = str(0)
	  entries = Entry.get_tagged_some(tag, 1)
	  older_url = "/tagged/"+tag+"/p2"

	heading = 'Tagged #'+tag
	temp_vals = { 'heading' : heading,
			'page' : page,
			'entries' : entries,
			'older_url' : older_url
			}

	display = JINJA_ENVIRONMENT.get_template('temp_display.html')
	self.response.write(display.render(temp_vals))

	footer(self)

class EntryHandler(webapp2.RequestHandler):
    def get(self, entry_key):
	header(self)
	entry = Entry.get_by_id(int(entry_key))
	temp_vals = { 'entry_key' : entry_key,
			'heading' : entry.title,
			'entry' : entry,
			}

	display = JINJA_ENVIRONMENT.get_template('temp_entry.html')
	self.response.write(display.render(temp_vals))
	footer(self)


application = webapp2.WSGIApplication([
    (r'/entry=(.*)', EntryHandler),
    (r'/user=(.*)/blog=(.*)/p([0-9]*)', BlogHandler),
    (r'/user=(.*)/blog=(.*)', BlogHandler),
    (r'/tagged/(.*)/p([0-9]*)', TagHandler),
    (r'/tagged/(.*)', TagHandler),
    (r'/user=(.*)/p([0-9]*)', UserHandler),
    (r'/user=(.*)', UserHandler),
    ('/p([0-9]*)', MainPage),
    ('/', MainPage),
], debug=True)
