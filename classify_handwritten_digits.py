from keras import Sequential
from keras.layers import Dense
from keras.layers import Conv2D
from keras.layers import MaxPooling2D
from keras.layers import AveragePooling2D
from keras.layers import Flatten
from keras.datasets import mnist
from keras.utils import to_categorical
from keras.losses import categorical_crossentropy
from keras.optimizers import SGD


batch_size = 128
num_classes = 10
epochs = 10

# Dimensions of input image
img_x, img_y = 28, 28

input_shape = (img_x, img_y, 1)

def build_model():
	model = Sequential()
	
	# First (hidden) layer. It consists of 1 convolutional and 1 (max) pooling layer
	model.add(Conv2D(64, kernel_size=(5,5), strides=(1,1), activation='relu', input_shape=input_shape))
	model.add(AveragePooling2D(pool_size=(2,2), strides=(2,2)))

	# Second layer.
	model.add(Conv2D(64, (5,5), strides=(2,2)))
	model.add(AveragePooling2D(pool_size=(2,2)))

	# Flattening layer.
	model.add(Flatten())

	# First dense layer.
	model.add(Dense(1000, activation='relu'))

	# Final regression layer.
	model.add(Dense(num_classes, activation='softmax'))

	model.compile(loss=categorical_crossentropy, optimizer=SGD(lr=0.01), metrics=['accuracy'])
	return model


def main():
	# Load MNIST dataset.
	(x_train, y_train), (x_test, y_test) = mnist.load_data()

	# Reshape data.
	x_train = x_train.reshape(x_train.shape[0], img_x, img_y, 1)
	x_test = x_test.reshape(x_test.shape[0], img_x, img_y, 1)

	# Convert classes to binary class matrices.
	y_train = to_categorical(y_train, num_classes)
	y_test = to_categorical(y_test, num_classes)


	# Convert data to 'float32' data type.
	x_train = x_train.astype('float32')
	x_test = x_test.astype('float32')
	x_train /= 255
	x_test /= 255

	model = build_model()
	
	# Train the model.
	model.fit(x_train, y_train, batch_size=batch_size, epochs=epochs, verbose=1, validation_data=(x_test, y_test))

	# Evaluate the model.
	score = model.evaluate(x_test, y_test, verbose=0)
	print('Test loss:', score[0])
	print('Test accuracy:', score[1])

	return


if __name__ == '__main__':
	main()