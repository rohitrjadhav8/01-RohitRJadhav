using System.Runtime.InteropServices;
using System.Windows.Forms;

namespace DotNetServerMaths {

    public interface IMath {
        void Addition(double real1, double img1,double real2,double img2,ref double answerReal,ref double answerImg);
        void Subtraction(double real1, double img1, double real2, double img2, ref double answerReal, ref double answerImg);
        void Multiplication(double real1, double img1, double real2, double img2, ref double answerReal, ref double answerImg);
        void Division(double real1,double img1,double real2,double img2,ref double answerReal,ref double answerImg);
    }


    [ClassInterface(ClassInterfaceType.None)]
    public class CMath : IMath {
        public CMath() {

        }

        public void Addition(double real1, double img1, double real2, double img2,ref double answerReal,ref double answerImg) {
            answerReal = real1 + real2;
            answerImg = img1 + img2;
        }

        public void Subtraction(double real1, double img1, double real2, double img2, ref double answerReal, ref double answerImg) {
            answerReal = real1 - real2;
            answerImg = img1 - img2;
       
        }

        public void Multiplication(double real1, double img1, double real2, double img2, ref double answerReal, ref double answerImg) {
            answerReal = ((real1 * real2) - (img1 * img2));
            answerImg = ((real1 * img2) - (img1 * real2));
       
        }

        public void Division(double real1, double img1, double real2, double img2, ref double answerReal, ref double answerImg) {
            answerReal = (real1 * real2 + img1 * img2) / (real2 * real2 + img2 * img2);
            answerImg = (img1 * real2 - real1 * img2) / (real2 * real2 + img2 * img2);
         

        }
    }
}