import unittest
from pyunitoken import *
import random

class PyUniTokenTest(unittest.TestCase):

    def setUp(self):
        self.handle = InitToken()
        self.assertIsNotNone(self.handle)

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

    def tearDown(self):
        self.assertRaises(IOError, TokenLogout(self.handle))
        self.assertRaises(IOError, CloseToken(self.handle))

if __name__ == "__main__":
    unittest.main()
