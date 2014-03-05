import os
import cgi
import urllib
import re

from google.appengine.api import users
from google.appengine.ext import ndb

import jinja2
import webapp2

from models import *
from main import *


def replace_html(string):
  newstring = re.sub(r'(\http[s]?://[^\s<>"]+|www\.[^\s<>"]+)', r'<a href="\1">\1</a>', string)
  string = re.sub(r'<a href="(\http[s]?://[^\s<>"]+|www\.[^\s<>"]+)">[^\s]+.jpg</a>', r'<img src="\1">', newstring)
  newstring = re.sub(r'<a href="(\http[s]?://[^\s<>"]+|www\.[^\s<>"]+)">[^\s]+.png</a>', r'<img src="\1">', string)
  string = re.sub(r'<a href="(\http[s]?://[^\s<>"]+|www\.[^\s<>"]+)">[^\s]+.gif</a>', r'<img src="\1">', newstring)
  return string

def strip_tags(temptags2):
  temptags2 = temptags2.split(',')
  for x in range(len(temptags2)):
	temptags2[x] = temptags2[x].strip()
	if temptags2[x] == '':
		temptags2[x] = None
  temptags2 = filter(None, temptags2)
  temptags2 = list(set(temptags2))
  return temptags2


class ManageBlog(webapp2.RequestHandler):

    def main(self):
	blog_name = self.request.get('blog_name', DEFAULT_BLOG_NAME)
	parent=blog_key(blog_name)
	user = str(users.get_current_user())
	blogs = Blog.get_author(user)
	entries = Entry.get_author(user)
	images = Image.get_author(user)

	man_vars = {'blogs' : blogs,
			'entries' : entries,
			'images' : images,
			}

	listing = JINJA_ENVIRONMENT.get_template('temp_manage.html')
	self.response.write(listing.render(man_vars))

    def get(self):
	header(self)
	if users.get_current_user():
		self.main()
	footer(self)

    def post(self):
	header(self)
	if users.get_current_user():
		self.main()
	footer(self)

class AddEntry(webapp2.RequestHandler):
    def main(self, blog):
	temp_vars = { 'action_url' : "/manage/add",
			'blog_name' : blog,
			'entry_title' : 'Entry Title',
			'entry_content' : 'Type entry here!',
			'entry_tags' : 'comma, delimited, tags',
			'new_blog' : 0,
			'submit_word' : 'Add Entry',
			}
	form = JINJA_ENVIRONMENT.get_template('temp_newblog.html')
	self.response.write(form.render(temp_vars))

    def get(self, blog=None):
	header(self)
	if users.get_current_user():
	  self.main(blog)
	footer(self)

    def post(self):
	if users.get_current_user():
		entry = Entry()
		entry.author = str(users.get_current_user())
		entry.blog = self.request.get('blogname')
		if entry.blog == '':
			entry.blog = 'Default_Blog_Title'
		entry.title = self.request.get('btitle')
		entry.body = replace_html(self.request.get('bcontent'))
		entry.tags = strip_tags(self.request.get('btags'))
		entry.put()

		if not Blog.has_blog(entry.author, entry.blog):
		  blog = Blog()
		  blog.author = entry.author
		  blog.name = entry.blog
		  blog.put()
	header(self)
	if users.get_current_user():
		self.response.write('<p class="main"><b>Success!</b></p>')
	footer(self)

class NewBlog(webapp2.RequestHandler):
    def main(self):
	temp_vars = { 'action_url' : "/manage/new",
			'blog_name' : 'BLOG_NAME',
			'entry_title' : 'Entry Title',
			'entry_content' : 'Type entry here!',
			'entry_tags' : 'comma, delimited, tags',
			'new_blog' : 1,
			'submit_word' : 'Add Blog',
			}
	form = JINJA_ENVIRONMENT.get_template('temp_newblog.html')
	self.response.write(form.render(temp_vars))

    def get(self):
	header(self)
	if users.get_current_user():
		self.main()
	footer(self)

    def post(self):
	entry = Entry()
	if users.get_current_user():
		entry.author = str(users.get_current_user())
		entry.blog = self.request.get('blogname')
		if entry.blog == '':
			entry.blog = 'Default_Blog_Title'
		entry.title = self.request.get('btitle')
		entry.body = replace_html(self.request.get('bcontent'))
		entry.tags = strip_tags(self.request.get('btags'))
		entry.put()

	if not Blog.has_blog(entry.author, entry.blog):
	  blog = Blog()
	  blog.author = entry.author
	  blog.name = entry.blog
	  blog.put()

	header(self)
	if users.get_current_user():
		self.response.write('<p class="main"><b>Success!</b></p>')
	footer(self)

class EditHandler(webapp2.RequestHandler):
    def main(self, key):
	entry = Entry.get_by_id(int(key))
	temp_vars = { 'action_url' : "/manage/edit="+key,
			'blog_name' : entry.blog,
			'entry_title' : entry.title,
			'entry_content' : entry.body,
			'entry_tags' : ', '.join(entry.tags),
			'new_blog' : 0,
			'submit_word' : 'Edit Entry',
			}
	form = JINJA_ENVIRONMENT.get_template('temp_newblog.html')
	self.response.write(form.render(temp_vars))

    def get(self, key):
	header(self)
	entry = Entry.get_by_id(int(key))
	user = str(users.get_current_user())
	if entry.author == user:
	    self.main(key)
	footer(self)

    def post(self, key):
	header(self)	
	entry = Entry.get_by_id(int(key))
	user = str(users.get_current_user())
	entry.title = self.request.get('btitle')
	if entry.title == '':
		entry.blog = 'Default_Entry_Title'
	entry.body = replace_html(self.request.get('bcontent'))
	entry.tags = strip_tags(self.request.get('btags'))
	entry.put()

	if not Blog.has_blog(entry.author, entry.blog):
	  blog = Blog()
	  blog.author = entry.author
	  blog.name = entry.blog
	  blog.put()

	if entry.author == user:
		self.response.write('<p class="main"><b>Success!</b></p>')

	footer(self)

class DeleteHandler(webapp2.RequestHandler):
    def get(self, key):
	header(self)
	if users.get_current_user():
	    entry = Entry.get_by_id(int(key))
	    user = str(users.get_current_user())
	    blog = entry.blog
	    if entry.author == user:
		entry.key.delete()
	    if len(Entry.query(ndb.AND(Entry.blog == blog,
					Entry.author == user)).fetch())<1:
		Blog.query(ndb.AND(Blog.name == blog,
					Blog.author == user)).get().key.delete()
	    self.response.write('<p class="main"><b>Success!</b></p>')
	else:
	    self.response.write('''<p class="main">You don't have permission!</p>''')
	footer(self)

application = webapp2.WSGIApplication([
    ('/manage/edit=(.*)', EditHandler),
    ('/manage/delete=(.*)', DeleteHandler),
    ('/manage/new/?', NewBlog),
    ('/manage/add', AddEntry),
    ('/manage/add/(.*)', AddEntry),
    ('/manage/?', ManageBlog),
], debug=True)
