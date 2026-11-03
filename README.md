# HumanGL
*This project has been created as part of the 42 curriculum by lboudjem and lalegran.*

# Description

HumanGL est une introduction pratique à la modélisation hiérarchique en OpenGL. Le projet met l'accent sur la composition de matrices pour assembler, lier et animer en temps réel les membres d'un modèle humanoïde.

# Instructions

# Fonctionnement

## La Modélisation Hiérarchique

Pour structurer et animer un personnage complexe sans positionner chaque cube de manière isolée dans l'espace, le projet repose sur une **modélisation hiérarchique** (ou *Scene Graph*).

Le principe consiste à organiser les différentes parties du corps sous forme d'un arbre de parenté. Chaque membre possède sa propre transformation locale (échelle, rotation.. etc), qui est ensuite multipliée par la matrice globale de son parent direct.

Dans notre cas, nous avons la structure suivante:

```text
Torse (Racine)
├── Tête
├── Bras
│   └── Avant-bras
└── Haut de la jambe
    └── Bas de jambe
```

Cette méthode permet ainsi d'appliquer en même temps des transformations au modèle en entier, mais aussi de manière isolée a des parties du corps.

De même, lorsqu'un parent bouge, tous ses enfants suivent automatiquement le mouvement, créant une impression de vrai corps articulé.

# Resources

- https://learnopengl.com/Guest-Articles/2021/Scene/Scene-Graph#:~:text=Guest%2DArticles%2F2021%2FScene%2FScene%2DGraph.%20In%20this%20chapter%2C%20we%20will%20talk,is%20also%20a%20child%20of%20my%20body.
- https://graphics.cs.cmu.edu/nsp/course/15-462/Spring04/slides/05-hierarchy.pdf#:~:text=*%20%E2%80%93%20for%20each%20leaf%20node%3A%20%C2%BB,%3D%20CM.%20*%20glRotatef(theta%2Cx%2Cy%2Cz)%2C%20glRotated(%E2%80%A6)%20axis%2Fangle%20rotate.
- https://ocw.mit.edu/courses/6-837-computer-graphics-fall-2012/59e619e792b658927c8e79f3ab0116ec_MIT6_837F12_assn2.pdf#:~:text=Generalize%20the%20code%20to%20handle%20multiple%20characters,your%20model%20on%20a%20floor%20using%20OpenGL.
