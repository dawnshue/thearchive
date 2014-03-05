from google.appengine.ext import ndb

DEFAULT_BLOG_NAME = 'untitled_blog'

def blog_key(blog_name=DEFAULT_BLOG_NAME):
    return ndb.Key('Blog', blog_name)

class Image(ndb.Model):
    ifile = ndb.BlobProperty()
    url = ndb.StringProperty()
    user = ndb.StringProperty()
    date = ndb.DateTimeProperty(auto_now_add=True)

    @classmethod
    def get_author(cls, user):
	q = Image.query(Image.user == user)
	q.order(-Image.date)
	return q.fetch()

class Entry(ndb.Model):
    """Models an individual Blog entry with the following properties."""
    author = ndb.StringProperty()
    blog = ndb.StringProperty()
    title = ndb.StringProperty()
    date = ndb.DateTimeProperty(auto_now_add=True)
    edit = ndb.DateTimeProperty(auto_now=True)
    body = ndb.TextProperty(indexed=False)
    tags = ndb.StringProperty(repeated=True)

    @classmethod
    def get_blog_all(cls, user, blog):
	q = Entry.query(ndb.AND(Entry.author == user, Entry.blog == blog))
	q.order(-Entry.date)
	return q.fetch()

    @classmethod
    def get_author(cls, user):
	q = Entry.query(Entry.author == user)
	q.order(-Entry.date)
	q.order(-Entry.blog)
	return q.fetch()

    @classmethod
    def get_author_blog(cls, user, blog, page):
	q = Entry.query(ndb.AND(Entry.author == user, Entry.blog == blog))
	q.order(-Entry.date)
	if int(page) > 0:
	  p = int(page)*10 - 10
	  return q.fetch(10, offset=p)
	else:
	  return q.fetch(10)

    @classmethod
    def get_author_some(cls, user, page):
	q = Entry.query(Entry.author == user)
	q.order(-Entry.date)
	if int(page) > 0:
	  p = int(page)*10 - 10
	  return q.fetch(10, offset=p)
	else:
	  return q.fetch(10)

    @classmethod
    def get_some(cls, page):
	q = Entry.query().order(-Entry.date)
	if int(page) > 0:
	  p = int(page)*10 - 10
	  return q.fetch(10, offset=p)
	else:
	  return q.fetch(10)

    @classmethod
    def get_tagged_some(cls, tag, page):
	q = Entry.query(Entry.tags == tag).order(-Entry.date)
	if int(page) > 0:
	  p = int(page)*10 - 10
	  return q.fetch(10, offset=p)
	else:
	  return q.fetch(10)

class Blog(ndb.Model):
    """Models sets of blogs"""
    author = ndb.StringProperty()
    name = ndb.StringProperty()

    @classmethod
    def get_author(cls, user):
	q = Blog.query(Blog.author == user)
	return q.fetch()

    @classmethod
    def has_blog(cls, user, blog):
	q = Blog.query(ndb.AND(Blog.author == user, Blog.name == blog))
	x = q.get()
	if x:
	  return True
	else:
	  return False
