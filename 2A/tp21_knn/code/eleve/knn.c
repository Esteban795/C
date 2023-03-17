#include "knn.h"

// Les  sont là pour éviter les avertissements dans les fonctions que
// vous n'avez pas encore écrites. Il faut les enlever au moment d'écrire
// la fonction.

int euclid_distance2(int *p1,  int *p2, size_t dim) {
    int sum = 0;
    for (unsigned i = 0;i < dim; i++){
        sum += (p1[i] - p2[i]) * (p1[i] - p2[i]);
    }
    return sum;
}

mnist_case_t **knn_kclosest(mnist_dataset_t dataset,mnist_picture_t candidate,unsigned k) {
    min_heap heap = mh_create(k);

    int pic_size = dataset.height * dataset.width;

    for (unsigned i = 0; i < k;i++){
        mh_insert(heap,-euclid_distance2(candidate,dataset.cases[i].picture,pic_size),&dataset.cases[i]);
    }

    for (unsigned i = k; i < dataset.case_num; i++){
        int dist = euclid_distance2(candidate,dataset.cases[i].picture,pic_size);
        double worst_dist = -mh_first(heap)->weight;
        if (dist < worst_dist) {
            mh_pop(heap);
            mh_insert(heap,-dist,&dataset.cases[i].picture);
        }
    }

    mnist_case_t** knn = malloc(sizeof(mnist_case_t*) * k);

    for (unsigned i = 0; i < k; i++){
        mnist_case_t* c = (mnist_case_t*)mh_pop(heap);
        knn[i] = c;
    }

    mh_free(heap);
    return knn;
}

mnist_label_t knn_majority(mnist_dataset_t dataset, mnist_picture_t candidate,mnist_case_t** kclosest, unsigned k) {
    mnist_label_t diff_label = dataset.max_label - dataset.min_label + 1;

    unsigned* poll = malloc(sizeof(poll) * k);
    for (unsigned i = 0; i < k;i++){
        poll[i] = 0;
    }

    for (unsigned i = 0; i < k;i++){
        poll[kclosest[i]->label - dataset.min_label] += 1;
    }

    mnist_label_t best_index = 0;
    unsigned best_value = poll[0];
    for (int i = 1; i < diff_label;i++){
        if (poll[i] > best_value){
            best_index = i;
            best_value = poll[i];
        }
    }
    free(poll);
    return best_index;
}

mnist_label_t knn_weighted_majority(mnist_dataset_t dataset, mnist_picture_t candidate, mnist_case_t** kclosest,unsigned k) {
    mnist_label_t diff_label = dataset.max_label - dataset.min_label + 1;
    int pic_size = dataset.height * dataset.width;
    double* poll = malloc(sizeof(poll) * k);
    for (unsigned i = 0; i < k;i++){
        poll[i] = 0;
    }

    for (unsigned i = 0; i < k;i++){
        poll[kclosest[i]->label - dataset.min_label] += 1.0/(1.0 + euclid_distance2(candidate,dataset.cases[i].picture,pic_size));
    }

    mnist_label_t best_index = 0;
    float best_value = poll[0];
    for (int i = 1; i < diff_label;i++){
        if (poll[i] > best_value){
            best_index = i;
            best_value = poll[i];
        }
    }
    free(poll);
    return best_index;
}


double* knn_confusion_matrix(knn_classifier_t classifier, mnist_dataset_t training, mnist_dataset_t testing, unsigned k, bool need_tree){
    mnist_label_t diff_label = training.max_label - training.min_label + 1;

    unsigned* confusion_matrix = malloc(sizeof(unsigned) * diff_label * diff_label); //row major matrix
    for (int i = 0; i < diff_label * diff_label;i++){
        confusion_matrix[i] = 0;
    }

    unsigned* tested_labels = malloc(sizeof(unsigned) * diff_label);
    for (int i = 0; i < testing.case_num;i++){
        mnist_label_t test_label = testing.cases[i].label;
        mnist_case_t** kclosest = knn_kclosest(training,testing.cases[i].picture,k);
        mnist_label_t predicted = classifier(training,testing.cases[i].picture,kclosest,k);

        confusion_matrix[test_label * diff_label + predicted] += 1;

        free(kclosest);
    }

    double* normalized_cm = malloc(sizeof(float) * diff_label * diff_label);
    for (int i = 0; i < diff_label;i++){
        normalized_cm[i] = confusion_matrix[i]
    }


}
