import Firebase
# Firebase.initialize("Sample")
Firebase.uploadfile("Sample.txt")
# Firebase.Dowmloadfile("abc.txt")
print(Firebase.geturl("abc.txt"))
# Firebase.deltefile("qw")
# print(Firebase.getconfig())

storage.child("path/to/file").update({"name": "new_filename"})

# Rename a folder
storage.child("path/to/folder/").update({"name": "new_foldername/"})