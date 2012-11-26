import unittest
from pyunitoken import *
import random

class PyUniTokenTest(unittest.TestCase):

    def setUp(self):
        self.handle = InitToken()
        self.assertIsNotNone(self.handle)

    def test_exceptions(self):
        with self.assertRaises(UniTokenNoFileError):
            raise UniTokenNoFileError

        with self.assertRaises(UniTokenError):
            raise UniTokenError

    def test_version(self):
        self.assertEqual(GetLibraryVersion(), 770)

    def test_userlevel(self):
        self.assertEqual(GetUserLevel(self.handle), 'guest')

    def test_permissions(self):

        with self.assertRaises(IOError) as cont:
            TokenLogin(self.handle, UT_USER_LEVEL_ADMIN, 'badpass')
        self.assertEqual(cont.exception.args, ("Password error",))
        TokenLogin(self.handle, UT_USER_LEVEL_ADMIN, 'admin')
        self.assertEqual(GetUserLevel(self.handle), 'admin')

    def test_changepin(self):
        TokenLogin(self.handle, UT_USER_LEVEL_ADMIN, 'admin')
        ChangePin(self.handle, UT_USER_LEVEL_ADMIN, 'admin', 'testpin')
        ChangePin(self.handle, UT_USER_LEVEL_ADMIN, 'testpin', 'admin')

    def test_softid(self):
        TokenLogin(self.handle, UT_USER_LEVEL_ADMIN, 'admin')
        softid = random.randint(1, 100500)
        SetSoftId(self.handle, softid)
        self.assertEqual(GetSoftId(self.handle), softid)

    def test_RSAencdec(self):
        TokenLogin(self.handle, UT_USER_LEVEL_ADMIN, 'admin')
        data = "".join([chr(x) for x in xrange(32,160)])
        pubhandle, prihandle = RSAGenKeyPair(self.handle, RSA_MODULUS_LEN_1024_BIT, RSA_PUBEXP_LEN_65537_BIT)
        self.assertIsNotNone(pubhandle)
        self.assertIsNotNone(prihandle)

        with self.assertRaisesRegexp(IOError, 'Data length is.*'):
            RSAPubKeyEncrypt(self.handle, pubhandle, data+"#")

        encdata = RSAPubKeyEncrypt(self.handle, pubhandle, data)
        self.assertIsNotNone(encdata)

        decdata = RSAPriKeyDecrypt(self.handle, prihandle, encdata)
        self.assertEqual(data, decdata)

    def test_format(self):
        TokenLogin(self.handle, UT_USER_LEVEL_ADMIN, 'admin')
        FormatToken(self.handle)

    def test_filesystem(self):
        TokenLogin(self.handle, UT_USER_LEVEL_ADMIN, 'admin')
#        self.assertEqual((0, 65536), FsGetSpace(self.handle))
#        self.assertEqual(0, FsGetFileCount(self.handle))
#        with self.assertRaises(UniTokenNoFileError):
#            FsGetFirstFileName(self.handle)
#        with self.assertRaises(UniTokenNoFileError):
#            FsGetNextFileName(self.handle)
#        self.assertEqual(0, FsCreateFile(self.handle, 'test1.bin', 64, FILE_PERMISSION_USER))
#        self.assertEqual(0, FsOpenFile(self.handle, 'test1.bin'))
        buf = "".join([chr(x) for x in xrange(32, 64+32)])
        FsWriteFile(self.handle, buf)
        self.assertEqual(0, FsDeleteFile(self.handle, 'test1.bin'))
    def test_fs2(self):

        TokenLogin(self.handle, UT_USER_LEVEL_ADMIN, 'admin')
        print "-%lu-  -%s-" % (FsGetFileCount(self.handle),  FsGetFirstFileName(self.handle))
        FsOpenFile(self.handle, '.Container')
        print FsReadFile(self.handle)

    def tearDown(self):
        TokenLogout(self.handle)
        CloseToken(self.handle)

if __name__ == "__main__":
    unittest.main()
