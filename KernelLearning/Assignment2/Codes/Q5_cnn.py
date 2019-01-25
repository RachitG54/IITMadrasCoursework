import scipy.io as spio
import numpy as np; 
import math
import random
import tensorflow as tf
import cPickle
import matplotlib.pyplot as plt
import matplotlib.cm as cm
from PIL import Image
sess = tf.InteractiveSession()
def loadmat(filename):
    '''
    this function should be called instead of direct spio.loadmat
    as it cures the problem of not properly recovering python dictionaries
    from mat files. It calls the function check keys to cure all entries
    which are still mat-objects
    '''
    data = spio.loadmat(filename, struct_as_record=False, squeeze_me=True)
    return _check_keys(data)

def _check_keys(dict):
    '''
    checks if entries in dictionary are mat-objects. If yes
    todict is called to change them to nested dictionaries
    '''
    for key in dict:
        if isinstance(dict[key], spio.matlab.mio5_params.mat_struct):
            dict[key] = _todict(dict[key])
    return dict        

def _todict(matobj):
    '''
    A recursive function which constructs from matobjects nested dictionaries
    '''
    dict = {}
    for strg in matobj._fieldnames:
        elem = matobj.__dict__[strg]
        if isinstance(elem, spio.matlab.mio5_params.mat_struct):
            dict[strg] = _todict(elem)
        else:
            dict[strg] = elem
    return dict

def diff(first, second):
        second = set(second)
        return [item for item in first if item not in second]

def get_actual_data():
    data = []
    label = []
    for i in range(total_label.size):
        if( total_label[i] == 40 ):
            data.append(total_data[i])
            label.append(0)
        elif( total_label[i] == 48 ):
            data.append(total_data[i])
            label.append(1)
        elif( total_label[i] == 56 ):
            data.append(total_data[i])
            label.append(2)
        elif( total_label[i] == 76 ):
            data.append(total_data[i])
            label.append(3)
        elif( total_label[i] == 93 ):
            data.append(total_data[i])
            label.append(4)
    return (data,label)

def split_data():
    split_indices = random.sample(range(467),281)
    train_data = []
    test_data = []
    train_label = []
    test_label = []
    val_data = []
    val_label = []  
    for i in split_indices:
        train_data.append(data[i])
        temp = np.zeros(5)
        temp[int(label[i])] = 1
        train_label.append(temp)
    train = (np.array(train_data),np.array(train_label))
    comb_data = []
    comb_label = []
    for i in diff(range(467),split_indices):
        comb_data.append(data[i])
        temp = np.zeros(5)
        temp[int(label[i])] = 1
        comb_label.append(temp)
        
    spl_indices = random.sample(range(186),84)
    for i in spl_indices:
        val_data.append(comb_data[i])
        val_label.append(comb_label[i])
    
    for i in diff(range(186),spl_indices):
        test_data.append(comb_data[i])
        test_label.append(comb_label[i])
        
    train = (np.array(train_data),np.array(train_label))
    val = (np.array(val_data),np.array(val_label))          
    test = (np.array(test_data),np.array(test_label))
    return (train,val,test)

def weight_variable(shape):
  initial = tf.truncated_normal(shape, stddev=0.1)
  return tf.Variable(initial)

def bias_variable(shape):
  initial = tf.constant(0.1, shape=shape)
  return tf.Variable(initial)

def conv2d(x, W):
  return tf.nn.conv2d(x, W, strides=[1, 1, 1, 1], padding='SAME')

def max_pool_2x2(x):
  return tf.nn.max_pool(x, ksize=[1, 2, 2, 1],
                        strides=[1, 2, 2, 1], padding='SAME')


mat = loadmat('2.Blackandwhiteimagedata/binaryData.mat')
total_data = mat['data']['data']
total_label = mat['data']['labels']
(data,label) = get_actual_data()
print len(data)
print label
(train_data,valid_data,test_data) = split_data()

x = tf.placeholder(tf.float32, shape=[None, 784])
y_ = tf.placeholder(tf.float32, shape=[None, 5])

x_image = tf.reshape(x, [-1,28,28,1])

W_conv1 = weight_variable([5, 5, 1, 10])
b_conv1 = bias_variable([10])

h_conv1 = tf.nn.relu(conv2d(x_image, W_conv1) + b_conv1)
h_pool1 = max_pool_2x2(h_conv1)

W_conv2 = weight_variable([5, 5, 10, 20])
b_conv2 = bias_variable([20])

h_conv2 = tf.nn.relu(conv2d(h_pool1, W_conv2) + b_conv2)
h_pool2 = max_pool_2x2(h_conv2)

h_pool2_flat = tf.reshape(h_pool2,[-1,20*7*7])

W_fc1 = weight_variable([20*7*7, 1024])
b_fc1 = bias_variable([1024])
h_fc1 = tf.nn.relu(tf.matmul(h_pool2_flat, W_fc1) + b_fc1)

keep_prob = tf.placeholder(tf.float32)
h_fc1_drop = tf.nn.dropout(h_fc1, keep_prob)

W_fc2 = weight_variable([1024, 5])
b_fc2 = bias_variable([5])

y_conv = tf.matmul(h_fc1_drop, W_fc2) + b_fc2

cross_entropy = tf.reduce_mean(
    tf.nn.softmax_cross_entropy_with_logits(labels=y_, logits=y_conv))
train_step = tf.train.AdamOptimizer(0.001).minimize(cross_entropy)
prediction = tf.argmax(y_conv,1)
labels = tf.argmax(y_,1)
correct_prediction = tf.equal(tf.argmax(y_conv,1), tf.argmax(y_,1))
accuracy = tf.reduce_mean(tf.cast(correct_prediction, tf.float32))
sess.run(tf.initialize_all_variables())

saver = tf.train.Saver()

#saver.restore(sess,save_path+'new-model-2')

epoch = 0
i=0
best_accuracy = 0
previousaccuracy = 0

try:
    count = 0
    
    for i in range(2):
        train_step.run(feed_dict={x: train_data[0], y_: train_data[1], keep_prob: 0.5})
        epoch = epoch + 100
        valid_accuracy = accuracy.eval(feed_dict={x:test_data[0], y_: test_data[1], keep_prob: 1.0})
        if( valid_accuracy >= best_accuracy ):
            best_accuracy = valid_accuracy
            #savePath = saver.save(sess,('./ModelCNN/best-model'),global_step=epoch)
        print("Epoch %d Validation accuracy %g"%(epoch,valid_accuracy))      
except:
    saver.restore(sess,savePath)
    
def f( i ):
    if( i > 0 ):
        return 1
    else:
        return 0
        
finder = lambda t : f(t)
vfunc = np.vectorize(finder)

test_accuracy = accuracy.eval(feed_dict={x:test_data[0], y_: test_data[1], keep_prob: 1.0})

print test_accuracy

print prediction.eval(feed_dict={x:test_data[0], y_: test_data[1], keep_prob: 1.0})
print labels.eval(feed_dict={x:test_data[0], y_: test_data[1], keep_prob: 1.0})

print prediction    
"""
lab = 0
while( lab < 5 ):
    for index in range(test_data[1].shape[0]):
        if( test_data[1][index][lab] == 1 ):
            imag = h_conv1.eval(feed_dict={x:test_data[0][index:index+1], y_: test_data[1][index:index+1], keep_prob: 1.0})
            imag2 = h_conv2.eval(feed_dict={x:test_data[0][index:index+1], y_: test_data[1][index:index+1], keep_prob: 1.0})
            for i in range(10):
                newArray = ((vfunc(imag[:,:,:,i].reshape(28,28))))
                plt.imsave('./CNN1/CNN1'+str(lab)+str(i)+'.png', np.array(newArray), cmap=cm.gray)
            for i in range(20):
                newArray = ((vfunc(imag2[:,:,:,i].reshape(14,14))))
                plt.imsave('./CNN2/CNN2'+str(lab)+str(i)+'.png', np.array(newArray), cmap=cm.gray)
            plt.imsave('./CNNcode/CNNcode'+str(lab)+'.png',np.array(test_data[0][index:index+1]).reshape(28,28),cmap=cm.gray)  
            lab = lab + 1
        if( lab >= 5):
            break 
"""            
'''
[0 0 1 0 0 0 0 2 0 0 0 0 0 0 0 0 0 0 0 0 0 1 1 1 1 1 1 1 1 3 1 1 0 0 1 1 4
 1 1 1 0 1 1 2 2 2 2 2 2 2 2 2 2 2 2 2 2 2 2 2 2 2 2 2 1 3 3 3 3 3 3 3 3 3
 3 3 3 3 4 4 4 4 4 4 4 4 4 4 4 4 4 4 4 4 4 4 4 4 4 4 4 4 4 4 4 4 4 4 4 4 4
 4 4 4 4 4 4 4 4 3 4 4 4 4 4]
[0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1
 1 1 1 1 1 1 2 2 2 2 2 2 2 2 2 2 2 2 2 2 2 2 2 2 2 2 2 2 3 3 3 3 3 3 3 3 3
 3 3 3 3 4 4 4 4 4 4 4 4 4 4 4 4 4 4 4 4 4 4 4 4 4 4 4 4 4 4 4 4 4 4 4 4 4
 4 4 4 4 4 4 4 4 4 4 4 4 4 4]
'''
#print test_data[0][1:2]


