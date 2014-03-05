import os
import urllib

from google.appengine.api import users
from google.appengine.ext import ndb
from google.appengine.api import images
import jinja2
import webapp2

from models import *
from main import *

class ImageHandler(webapp2.RequestHandler):
    def get(self, url):
	picture = Image.query(Image.url == url).get()
	#picture = Image.get_by_id(int(url))
	if picture.ifile:
	    self.response.headers['Content-Type'] = 'image/png'
	    self.response.out.write(picture.ifile)

class Uploader(webapp2.RequestHandler):
    def get(self):
	header(self)
	display = JINJA_ENVIRONMENT.get_template('temp_upload.html')
	self.response.write(display.render())
	footer(self)
    def post(self):
	header(self)
	if users.get_current_user():
		res = self.request.get('img')
		image = Image()
		#image.ifile = ndb.BlobProperty(res)
		image.ifile = res
		image.user = str(users.get_current_user())
		#image.url = image.user+"_"+self.request.params['img'].filename
		image_key = image.put()
		image = Image.get_by_id(image_key.id())
		image.url = str(image_key.id())+"_"+self.request.params['img'].filename
		image.put()
	self.response.write('<p class="main">Upload Success!</p>')
	footer(self)

class DeleteImage(webapp2.RequestHandler):
    def get(self, key):
	header(self)
	if users.get_current_user():
	    img = Image.get_by_id(int(key))
	    user = str(users.get_current_user())
	    if img.user == user:
		img.key.delete()
	    self.response.write('<p class="main"><b>Success!</b></p>')
	else:
	    self.response.write('''<p class="main">You don't have permission!</p>''')
	footer(self)


application = webapp2.WSGIApplication([
    ('/upload', Uploader),
    ('/img/delete/(.*)', DeleteImage),
    ('/img/(.*)', ImageHandler),
], debug=True)
