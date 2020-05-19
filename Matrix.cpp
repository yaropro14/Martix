#include <cstdio>
#include <thread>
#include <vector>


struct Matrix   {
    
	double  * val = nullptr;
	size_t size;

	Matrix(size_t size, double  * nums = nullptr) :
		size(size), val(new double[size * size])    {
            
		if (nums == nullptr)   {
			for (size_t i = 0; i < size * size; ++i)    {
				val[i] = 0;
			}	
		}
            
        else  {
			for (size_t i = 0; i < size * size; ++i)    {
				val[i] = nums[i];
			}
		}
	}
    
    
    void print()    {
        FILE * f_out = fopen("out.txt", "w");
        for (size_t x = 0; x < size; x ++)   {
            
            for (size_t y = 0; y < size; y ++)   {
                fprintf(f_out, "%lf ", val[x * size + y]);
            }
            
            fprintf(f_out, "\n");
        }
    }
    

	~Matrix() {
		delete[] val;
		val = nullptr;
	}
};


void thread_mult(Matrix* a, Matrix* b, Matrix* c, size_t X_size, size_t Y_size, size_t b_size);
void matrix_mult(Matrix* a, Matrix* b, Matrix* c);
int min(int x, int y);


int main(int argc, char ** argv)    {
    
    if(argc < 2)   {
        printf("ERROR: not enough arguments!\n");
        return 0;
    }
    
    FILE * f_in;
    f_in = fopen(argv[1], "r");
    
    size_t size = 0;
    fscanf(f_in, "%zd", &size);
    if(size < 2)    {
        printf("ERROR: fiew arguments");
        return 0;
    }
    
	double aValue[size * size];
    for(int i = 0; i < size * size; i++)    {
        fscanf(f_in, "%lf", &aValue[i]);
    }
	Matrix a(size, aValue);
    
	double bValue[size * size];
    for(int i = 0; i < size * size; i++)    {
           fscanf(f_in, "%lf", &bValue[i]);
    }
	Matrix b(size, bValue);
    
	Matrix c(size);

	matrix_mult(&b, &a, &c);
    
    c.print();

	return 0;
}


int min(int x, int y)   {
    return ((x)>(y))?(y):(x);
}


void matrix_mult(Matrix* a, Matrix* b, Matrix* c)   {
    std::vector<std::thread> thr;
    size_t b_size = ((a->size % 2) == 0) ? 2 : 3;
    
    for (size_t X = 0; X < a -> size; X += b_size)   {
        
        for (size_t Y = 0; Y < b -> size; Y += b_size)   {
            thr.push_back(std::thread(thread_mult, a, b, c, X, Y, b_size));
        }
        
    }
    
    for (auto& thread : thr)    {
        thread.join();
    }
}


void thread_mult(Matrix* a, Matrix* b, Matrix* c, size_t X_size, size_t Y_size, size_t b_size)    {
    size_t endX = min(X_size + b_size, a -> size);
    size_t endY = min(Y_size + b_size, b -> size);

    for (size_t x = X_size; x < endX; ++x) {
        
        for (size_t y = Y_size; y < endY; ++y) {
            
            for (size_t l = 0; l < a -> size; ++l)  {
                c->val[x * c->size + y] += a->val[l * a->size + y] * b->val[x * b->size + l];
            }
            
        }
    }
    
}
