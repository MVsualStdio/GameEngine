import shutil
import os


class Builder:
    def __init__(self,filePath:str):
        self.m_filePath = filePath


    def copyHLSL(self):
        print('copy:')
        for path in os.listdir(self.m_filePath):
            print(path)
            shutil.copyfile(self.m_filePath +'/' + path, './build/'+ path)
            shutil.copyfile(self.m_filePath +'/' + path, './build/Debug/'+ path)

    def cmake(self):
        os.system('cd out')
        os.system('cmake ..')

if __name__ == "__main__":
    builder = Builder('./hlsl')
    builder.copyHLSL()
