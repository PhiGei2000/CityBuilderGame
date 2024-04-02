# Road intersections

## Representations of road graph edges

A edge $r$ in the road graph is represented by two positions $\vec r_1,\vec r_2\in \mathbb{Z}^2$. Both of these positions must fullfill either $\vec r_1-\vec r_2\parallel \vec e_x$ or $\vec r_1-\vec r_2\parallel \vec e_y$. Another representation is given by $r=\left\{\vec r(\lambda)=\vec r_0 + \lambda \vec l\mid \lambda\in [0;l]\cap \mathbb{N_0}\right\}$ where $\vec r_0=min(\vec r_1,\vec r_2)$, $l=\|\vec r_2-\vec r_1\|$ and $\vec l =abs(\vec r_1-\vec r_2)$.

## Intersections

To check if two edges $\vec r_1(\lambda)$ and $\vec r_2(\lambda)$ intersect, one has to solve the equation $\vec r_1(\lambda_1)=\vec r_2(\lambda_2)$ for $\lambda_1$ and $\lambda_2$. To do this it is useful to reconsider the matrix $L=\begin{pmatrix}\vec l_1, \vec l_2\end{pmatrix}$. If the determinant of this matrix is zero, then the vectors $\vec l_1, \vec l_2$ are parallel and so are the edges. If this is the case one can write $\vec l_1$ as $\vec l_1=\alpha\vec l_2$ for an $\alpha\in\mathbb{Z}$. The the equation simplifies to $(\alpha\lambda_1-\lambda_2)\vec l_2=\vec r_{2,0}-\vec r_{1,0}=\Delta\vec r_0$. This has a solution if and only if $\det(\vec l_2,\Delta\vec r_0)=0$.