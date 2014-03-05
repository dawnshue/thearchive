import os
import urllib

from google.appengine.api import users
from google.appengine.ext import ndb
from google.appengine.api import images
import jinja2
import webapp2

from models import *
from main import *


class Converter(webapp2.RequestHandler):
    def get(self, user, blog):
	blog_url = 'http://vds229-ost.appspot.com/user='+user+'/blog='+blog
	entries = Entry.get_blog_all(user, blog)
	temp_vars = { 'title' : blog,
			'blog_url' : blog_url,
			'author' : user,
			'entries' : entries
			}
	self.response.headers['Content-Type'] = 'text/xml'
	display = JINJA_ENVIRONMENT.get_template('temp_rss.xml')
	self.response.write(display.render(temp_vars))


application = webapp2.WSGIApplication([
    ('/rss/user=(.*)/blog=(.*)/rss.xml', Converter),
], debug=True)
