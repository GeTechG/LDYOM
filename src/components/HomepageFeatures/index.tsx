import clsx from 'clsx';
import Heading from '@theme/Heading';
import styles from './styles.module.css';

type FeatureItem = {
  title: string;
  Svg: React.ComponentType<React.ComponentProps<'svg'>>;
  description: JSX.Element;
};

const FeatureList: FeatureItem[] = [
    {
        title: 'Flexible Mission Creation',
        Svg: require('@site/static/img/dyom_logo.svg').default,
        description: (
            <>
                LDYOM offers a versatile platform for creating missions in Grand Theft Auto: San Andreas. It serves as a spiritual successor to DYOM, delivered as external ASI plugins in C++, providing modularity and flexibility.
            </>
        ),
    },
    {
        title: 'Advanced Scene Design',
        Svg: require('@site/static/img/mind_map.svg').default,
        description: (
            <>
                Create both linear and non-linear missions with ease using LDYOM's scene system. Transition conditions between scenes allow for complex mission structures, while a node editor empowers designers to visually program entity behaviors.
            </>
        ),
    },
    {
        title: 'Extensible Functionality',
        Svg: require('@site/static/img/pair_programming.svg').default,
        description: (
            <>
                Extend LDYOM's capabilities with ease. Utilize its Lua scripting engine to craft addons, mission scripts, and new nodes, enabling limitless possibilities for mission creation and customization.
            </>
        ),
    },
];

function Feature({title, Svg, description}: FeatureItem) {
  return (
    <div className={clsx('col col--4')}>
      <div className="text--center">
        <Svg className={styles.featureSvg} role="img" />
      </div>
      <div className="text--center padding-horiz--md">
        <Heading as="h3">{title}</Heading>
        <p>{description}</p>
      </div>
    </div>
  );
}

export default function HomepageFeatures(): JSX.Element {
  return (
    <section className={styles.features}>
      <div className="container">
        <div className="row">
          {FeatureList.map((props, idx) => (
            <Feature key={idx} {...props} />
          ))}
        </div>
      </div>
    </section>
  );
}
